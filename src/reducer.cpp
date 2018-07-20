#include "reducer.h"
#include "utils.h"


/**
TODO internal structure of the file block format
i.e., uint64_t at the beginning for valid bytes

**/
template <class K, class V>
SortReduceReducer::StreamMergeReducer<K,V>::StreamMergeReducer() {
	this->m_out_offset = 0;
	this->m_out_file_offset = 0;
	this->mp_buffer_manager = AlignedBufferManager::GetInstance(1);
	
	m_out_block = mp_buffer_manager->GetBuffer();
	while (!m_out_block.valid) {
		mp_temp_file_manager->CheckDone();
		m_out_block = mp_buffer_manager->GetBuffer();
	}
}

template <class K, class V>
inline void
SortReduceReducer::StreamMergeReducer<K,V>::EmitKv(K key, V val) {
	const size_t kv_bytes = sizeof(K)+sizeof(V);

	if ( m_out_offset + kv_bytes <= m_out_block.bytes ) {
		StreamMergeReducer<K,V>::EncodeKvp(m_out_block.buffer, m_out_offset, key, val);
		m_out_offset += kv_bytes;

	} else {
		size_t available = m_out_block.bytes - m_out_offset;
		size_t debt = kv_bytes - available;
		if ( available >= sizeof(K) ) {
			StreamMergeReducer<K,V>::EncodeKey(m_out_block.buffer, m_out_offset, key);
			available -= sizeof(K);
			memcpy(((uint8_t*)m_out_block.buffer)+m_out_offset+sizeof(K), &val, available);

			m_out_block.valid_bytes = m_out_block.bytes;
			while ( !this->mp_temp_file_manager->Write(this->m_out_file, m_out_block, m_out_file_offset) ) usleep(50);
			m_out_file_offset += m_out_block.valid_bytes;
			m_out_block = mp_buffer_manager->GetBuffer();
			while (!m_out_block.valid) {
				this->mp_temp_file_manager->CheckDone();
				m_out_block = mp_buffer_manager->GetBuffer();
			}

			memcpy(m_out_block.buffer, ((uint8_t*)&val)+available, debt);
			m_out_offset = debt;
		} else {
			memcpy(((uint8_t*)m_out_block.buffer)+m_out_offset, &key, available);

			m_out_block.valid_bytes = m_out_block.bytes;
			while ( !this->mp_temp_file_manager->Write(this->m_out_file, m_out_block, m_out_file_offset) ) usleep(50);
			m_out_file_offset += m_out_block.valid_bytes;

			m_out_block = mp_buffer_manager->GetBuffer();
			while (!m_out_block.valid) {
				this->mp_temp_file_manager->CheckDone();
				m_out_block = mp_buffer_manager->GetBuffer();
			}

			memcpy(m_out_block.buffer, ((uint8_t*)&key)+available, sizeof(K)-available);
			StreamMergeReducer<K,V>::EncodeVal(m_out_block.buffer, sizeof(K)-available, val);
			m_out_offset = debt;
		}

	}
}

template <class K, class V>
void
SortReduceReducer::StreamMergeReducer<K,V>::EmitFlush() {
	m_out_block.valid_bytes = m_out_offset;
	while ( !this->mp_temp_file_manager->Write(this->m_out_file, m_out_block, m_out_file_offset) ) usleep(50);
	m_out_file_offset += m_out_block.valid_bytes;

	while (this->mp_temp_file_manager->CountInFlight() > 0 ) this->mp_temp_file_manager->CheckDone();
}

template <class K, class V>
inline K
SortReduceReducer::StreamMergeReducer<K,V>::DecodeKey(void* buffer, size_t offset) {
	return *(K*)(((uint8_t*)buffer)+offset);
}

template <class K, class V>
inline V
SortReduceReducer::StreamMergeReducer<K,V>::DecodeVal(void* buffer, size_t offset) {
	return *(V*)(((uint8_t*)buffer)+offset);
}
template <class K, class V>
inline void
SortReduceReducer::StreamMergeReducer<K,V>::EncodeKvp(void* buffer, size_t offset, K key, V val) {
	*(K*)(((uint8_t*)buffer)+offset) = key;
	*(V*)(((uint8_t*)buffer)+offset+sizeof(K)) = val;
}
template <class K, class V>
inline void 
SortReduceReducer::StreamMergeReducer<K,V>::EncodeKey(void* buffer, size_t offset, K key) {
	*(K*)(((uint8_t*)buffer)+offset) = key;
}

template <class K, class V>
inline void 
SortReduceReducer::StreamMergeReducer<K,V>::EncodeVal(void* buffer, size_t offset, V val) {
	*(V*)(((uint8_t*)buffer)+offset) = val;
}
	
template <class K, class V>
size_t 
SortReduceReducer::ReduceInBuffer(V (*update)(V,V), void* buffer, size_t bytes) {
	printf( "Reducing!! %s:%d\n", __FILE__, __LINE__ );
	size_t key_bytes = sizeof(K);
	size_t val_bytes = sizeof(V);
	size_t in_offset = 0;
	size_t out_offset = key_bytes+val_bytes;
	while ( in_offset < bytes ) {
		K in_key = *(K*)(((uint8_t*)buffer)+in_offset);
		K out_key = *(K*)(((uint8_t*)buffer)+out_offset);
		if ( in_key == out_key ) {
			V* p_in_val = (V*)(((uint8_t*)buffer)+in_offset+key_bytes);
			V in_val = *p_in_val;
			V out_val = *(V*)(((uint8_t*)buffer)+out_offset+key_bytes);
			*p_in_val = update(in_val,out_val);
		} else {
			out_offset += key_bytes+val_bytes;
		}
		in_offset += key_bytes+val_bytes;
	}
	return out_offset;
}

template <class K, class V>
SortReduceReducer::StreamMergeReducer_SinglePriority<K,V>::StreamMergeReducer_SinglePriority(V (*update)(V,V), std::string temp_directory, std::string filename) : SortReduceReducer::StreamMergeReducer<K,V>() {
	this->m_done = false;
	this->m_started = false;

	//this->ms_temp_directory = temp_directory;
	this->mp_update = update;

	this->mp_temp_file_manager = new TempFileManager(temp_directory);
	
	this->m_out_file = this->mp_temp_file_manager->CreateEmptyFile(filename);
}

template <class K, class V>
SortReduceReducer::StreamMergeReducer_SinglePriority<K,V>::~StreamMergeReducer_SinglePriority() {
	m_worker_thread.join();
	delete this->mp_temp_file_manager;
	//printf( "Worker thread joined\n" );
}

template <class K, class V>
void
SortReduceReducer::StreamMergeReducer_SinglePriority<K,V>::PutBlock(SortReduceTypes::Block block) {
	if ( m_started ) {
		fprintf(stderr, "Attempting to add data source to started reducer\n" );
		return;
	}

	DataSource source;
	source.from_file = false;
	source.block = block;
	source.file = NULL;
	mv_input_sources.push_back(source);
}

template <class K, class V>
void
SortReduceReducer::StreamMergeReducer_SinglePriority<K,V>::PutFile(SortReduceTypes::File* file) {
	if ( m_started ) {
		fprintf(stderr, "Attempting to add data source to started reducer\n" );
		return;
	}

	DataSource source;
	source.from_file = true;
	source.block.valid = false;
	source.file = file;
	mv_input_sources.push_back(source);
}

template <class K, class V>
void
SortReduceReducer::StreamMergeReducer_SinglePriority<K,V>::Start() {
	m_started = true;

	//printf( "StreamMergeReducer_SinglePriority started with %lu inputs\n", mv_input_sources.size() ); fflush(stdout);

	m_worker_thread = std::thread(&StreamMergeReducer_SinglePriority<K,V>::WorkerThread,this);
}


template <class K, class V>
void
SortReduceReducer::StreamMergeReducer_SinglePriority<K,V>::WorkerThread() {

	const int file_reads_inflight_target = 4;
	const size_t kv_bytes = sizeof(K)+sizeof(V);
	const int source_count = mv_input_sources.size();

	std::vector<int> reads_inflight(source_count, 0);
	int total_reads_inflight = 0;

	std::vector<std::queue<SortReduceTypes::Block>> ready_blocks;
	for ( int i = 0; i < source_count; i++ ) {
		ready_blocks.push_back(std::queue<SortReduceTypes::Block>());
	}
	
	std::vector<size_t> read_offset(source_count, 0);
	std::vector<size_t> file_offset(source_count, 0);

	// marked when last read request has been issued
	std::vector<bool> file_eof(source_count, false);
	
	AlignedBufferManager* buffer_manager = AlignedBufferManager::GetInstance(1);

	std::queue<std::tuple<int,SortReduceTypes::Block>> read_request_order;


	for ( int i = 0; i < source_count; i++ ) {
		if ( mv_input_sources[i].from_file ) {
		
			SortReduceTypes::File* file = mv_input_sources[i].file;
			SortReduceTypes::Block block = buffer_manager->WaitBuffer();

			//printf( "Block %ld %d\n",block.bytes,block.managed_idx ); fflush(stdout);

			while ( 0 > this->mp_temp_file_manager->Read(file, file_offset[i], block.bytes, block.buffer) );
			reads_inflight[i] ++;
			total_reads_inflight ++;

			if ( block.bytes + file_offset[i] <= file->bytes ) {
				block.valid_bytes = block.bytes;
			} else {
				block.valid_bytes = file->bytes - file_offset[i];
				file_eof[i] = true;
			}

			file_offset[i] += block.bytes;

			read_request_order.push(std::make_tuple(i,block));
		} else {
			SortReduceTypes::Block block = mv_input_sources[i].block;
			ready_blocks[i].push(block);
		}
	}

	while (total_reads_inflight > 0 ) {
		int read_done = this->mp_temp_file_manager->ReadStatus(true);
		total_reads_inflight -= read_done;

		for ( int i = 0; i < read_done; i++ ) {
			std::tuple<int,SortReduceTypes::Block> req = read_request_order.front();
			read_request_order.pop();
			int dst = std::get<0>(req);
			SortReduceTypes::Block block = std::get<1>(req);

			reads_inflight[dst] --;

			ready_blocks[dst].push(block);
		}

		if ( total_reads_inflight < 0 ) {
			fprintf(stderr, "Total_reads_inflight is negative! %d %s:%d\n", total_reads_inflight, __FILE__, __LINE__ );
		}
	}

	// ready_blocks is fully populated

	for ( int i = 0; i < source_count; i++ ) {
		if ( ready_blocks[i].empty() ) {
			fprintf(stderr, "ready_blocks is not fully populated! %s:%d\n", __FILE__, __LINE__ );
			continue;
		} 

		SortReduceTypes::Block block = ready_blocks[i].front();
			
		K key = StreamMergeReducer<K,V>::DecodeKey(block.buffer, 0);
		V val = StreamMergeReducer<K,V>::DecodeVal(block.buffer, sizeof(K));
		KvPairSrc kvp;
		kvp.key = key;
		kvp.val = val;
		kvp.src = i;
		m_priority_queue.push(kvp);
		//printf( "Pushing initial values %lx %lx @ %d\n", (uint64_t)key, (uint64_t)val, i );
		//fflush(stdout);

		read_offset[i] += kv_bytes;
	}
	
	K last_key = 0;
	V last_val = 0;
	bool first_kvp = true;

	while (!m_priority_queue.empty() ) {
		KvPairSrc kvp = m_priority_queue.top();
		m_priority_queue.pop();
		int src = kvp.src;

		//printf( "Next: %lx %lx from %d\n", (uint64_t)kvp.key, (uint64_t)kvp.val, src );

		if ( first_kvp ) {
			last_key = kvp.key;
			last_val = kvp.val;
			first_kvp = false;
		} else {
			if ( last_key == kvp.key ) {
				last_val = mp_update(last_val, kvp.val);
			} else {
				if ( last_key > kvp.key ) {
					printf( "StreamMergeReducer_SinglePriority order wrong! %lx %lx\n", (uint64_t)last_key, (uint64_t)kvp.key );
				}
				this->EmitKv(last_key, last_val);
				last_key = kvp.key;
				last_val = kvp.val;
			}
		}
		
		SortReduceTypes::Block block = ready_blocks[src].front();
			
		if ( block.valid_bytes >= read_offset[src] + kv_bytes ) {
			K key = StreamMergeReducer<K,V>::DecodeKey(block.buffer, read_offset[src]);
			V val = StreamMergeReducer<K,V>::DecodeVal(block.buffer, read_offset[src]+sizeof(K));
			KvPairSrc kvp;
			kvp.key = key;
			kvp.val = val;
			kvp.src = src;
			m_priority_queue.push(kvp);

			read_offset[src] += kv_bytes;
		} else if ( mv_input_sources[src].from_file ) {
			while ( !file_eof[src] && ready_blocks[src].size() + reads_inflight[src] < file_reads_inflight_target ) {
				SortReduceTypes::File* file = mv_input_sources[src].file;
				SortReduceTypes::Block block = buffer_manager->GetBuffer();
				while (!block.valid) {
					this->mp_temp_file_manager->CheckDone();
					block = buffer_manager->GetBuffer();
				}

				while ( 0 > this->mp_temp_file_manager->Read(file, file_offset[src], block.bytes, block.buffer) );

				if ( block.bytes + file_offset[src] <= file->bytes ) {
					block.valid_bytes = block.bytes;
				} else {
					block.valid_bytes = file->bytes - file_offset[src];
					file_eof[src] = true;
					//printf( "Reached eof %2d!!\n", src ); 
				}
				//printf( "Read req! %2d %lu\n",src, file_offset[src] );
				//fflush(stdout);
				
				file_offset[src] += block.bytes;
				reads_inflight[src] ++;
				total_reads_inflight ++;

				read_request_order.push(std::make_tuple(src,block));
				//printf("Sent read requests\n");fflush(stdout);
			}

			while (ready_blocks[src].size() < 2 && reads_inflight[src] > 0 ) {
				int read_done = this->mp_temp_file_manager->ReadStatus(true);
				total_reads_inflight -= read_done;

				for ( int i = 0; i < read_done; i++ ) {
					std::tuple<int,SortReduceTypes::Block> req = read_request_order.front();
					read_request_order.pop();
					int dst = std::get<0>(req);
					SortReduceTypes::Block block = std::get<1>(req);

					reads_inflight[dst] --;

					ready_blocks[dst].push(block);

					//printf( "File read %d\n", src ); fflush(stdout);
				}

				if ( total_reads_inflight < 0 ) {
					fprintf(stderr, "Total_reads_inflight is negative! %d %s:%d\n", total_reads_inflight, __FILE__, __LINE__ );
				}
			}

			SortReduceTypes::Block cur_block = ready_blocks[src].front();
			if ( ready_blocks[src].size() > 1 ) { // If not, we're done!
				size_t debt = read_offset[src] + kv_bytes - block.valid_bytes;
				size_t available = kv_bytes - debt;

				K key;
				V val;
	
				if ( available >= sizeof(K) ) {
					// cut within V
					key = StreamMergeReducer<K,V>::DecodeKey(block.buffer, read_offset[src]);
					read_offset[src] += sizeof(K);
					memcpy(&val, ((uint8_t*)block.buffer+read_offset[src]), available-sizeof(K));
					ready_blocks[src].pop();
					SortReduceTypes::Block block = ready_blocks[src].front();
					memcpy(((uint8_t*)&val)+available-sizeof(K), block.buffer, debt);
					read_offset[src] = debt;
				} else {
					// cut within K
					memcpy(&key, ((uint8_t*)block.buffer+read_offset[src]), available);
					ready_blocks[src].pop();
					SortReduceTypes::Block block = ready_blocks[src].front();

					memcpy(((uint8_t*)&key)+available, block.buffer, sizeof(K)-available);
					read_offset[src] = sizeof(K)-available;
					val = StreamMergeReducer<K,V>::DecodeVal(block.buffer, read_offset[src]);
					read_offset[src] += sizeof(V);
				}

				KvPairSrc kvp;
				kvp.key = key;
				kvp.val = val;
				kvp.src = src;
				m_priority_queue.push(kvp);
			} else {
				SortReduceTypes::File* file = mv_input_sources[src].file;
				this->mp_temp_file_manager->Close(file->fd);
				//printf( "File closed! %2d\n", src ); fflush(stdout);
			}

			buffer_manager->ReturnBuffer(cur_block);
		} else {
			// If from in-memory block, there is no more!
			SortReduceTypes::Block block = mv_input_sources[src].block;
			if ( block.managed ) {
				//printf( "Returning managed in-memory sort buffer\n" ); fflush(stdout);
				AlignedBufferManager* managed_buffers = AlignedBufferManager::GetInstance(0);
				managed_buffers->ReturnBuffer(block);
			} else {
				//printf( "Freeing in-memory sort buffer\n" ); fflush(stdout);
				free(block.buffer);
			}
		}
	}

	//flush out_block
	this->EmitKv(last_key, last_val);
	this->EmitFlush();

	//printf( "Reducer done!\n" ); fflush(stdout);

	m_done = true;
}

template class SortReduceReducer::StreamMergeReducer<uint32_t, uint32_t>;
template class SortReduceReducer::StreamMergeReducer<uint32_t, uint64_t>;
template class SortReduceReducer::StreamMergeReducer<uint64_t, uint32_t>;
template class SortReduceReducer::StreamMergeReducer<uint64_t, uint64_t>;


template class SortReduceReducer::StreamMergeReducer_SinglePriority<uint32_t, uint32_t>;
template class SortReduceReducer::StreamMergeReducer_SinglePriority<uint32_t, uint64_t>;
template class SortReduceReducer::StreamMergeReducer_SinglePriority<uint64_t, uint32_t>;
template class SortReduceReducer::StreamMergeReducer_SinglePriority<uint64_t, uint64_t>;

template size_t SortReduceReducer::ReduceInBuffer<uint32_t, uint32_t>(uint32_t (*update)(uint32_t,uint32_t), void* buffer, size_t bytes);
template size_t SortReduceReducer::ReduceInBuffer<uint32_t, uint64_t>(uint64_t (*update)(uint64_t,uint64_t), void* buffer, size_t bytes);
template size_t SortReduceReducer::ReduceInBuffer<uint64_t, uint32_t>(uint32_t (*update)(uint32_t,uint32_t), void* buffer, size_t bytes);
template size_t SortReduceReducer::ReduceInBuffer<uint64_t, uint64_t>(uint64_t (*update)(uint64_t,uint64_t), void* buffer, size_t bytes);



