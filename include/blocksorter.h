#ifndef __BLOCKSORT_H__
#define __BLOCKSORT_H__

#include <queue>
#include <string>
#include <stack>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <algorithm>

#include <time.h>

#include "types.h"
#include "utils.h"
#include "tempfilemanager.h"
#include "reducer.h"

template <class K, class V>
class BlockSorterThread {
public:
	BlockSorterThread(SortReduceTypes::Config<K,V>* config, SortReduceUtils::MutexedQueue<SortReduceTypes::Block>* buffer_queue_in, SortReduceUtils::MutexedQueue<SortReduceTypes::Block>* buffer_queue_out, TempFileManager* file_manager, SortReduceUtils::MutexedQueue<SortReduceTypes::File>* temp_files, SortReduceTypes::ComponentStatus* status);
	void Exit();

	typedef struct __attribute__ ((__packed__)) {K key; V val;} KvPair;
private:
	void SorterThread();
	std::thread m_thread;
	bool m_exit;


	//template <class tKV>
	static void SortKV(void* buffer, size_t bytes);

	//template <class tKV>
	static bool CompareKV(KvPair a, KvPair b);


	SortReduceUtils::MutexedQueue<SortReduceTypes::Block>* m_buffer_queue_in;
	SortReduceUtils::MutexedQueue<SortReduceTypes::Block>* m_buffer_queue_out;


	TempFileManager* mp_file_manager;
	
	SortReduceUtils::MutexedQueue<SortReduceTypes::File>* mq_temp_files;

	SortReduceTypes::ComponentStatus* mp_status;
	SortReduceTypes::Config<K,V>* mp_config;
};

template <class K, class V>
class BlockSorter {
public:
	BlockSorter(SortReduceTypes::Config<K,V>* config, SortReduceUtils::MutexedQueue<SortReduceTypes::File>* temp_files, std::string temp_path, size_t buffer_size, int buffer_count, int max_threads);
	~BlockSorter();

	void PutBlock(void* buffer, size_t bytes);
	SortReduceTypes::Block GetBlock();

	//size_t GetBlock(void* buffer);
	void CheckSpawnThreads();

	// Does not include managed blocks
	size_t BytesInFlight();


	SortReduceTypes::Block GetFreeManagedBlock();
	void PutManagedBlock(SortReduceTypes::Block block);



private:
	size_t m_maximum_threads;
	size_t m_in_queue_spawn_limit_blocks;
	size_t m_out_queue_spawn_limit_blocks;
	std::vector<BlockSorterThread<K,V>*> mv_sorter_threads;
	timespec m_last_thread_check_time;

	SortReduceUtils::MutexedQueue<SortReduceTypes::Block>* m_buffer_queue_in;
	SortReduceUtils::MutexedQueue<SortReduceTypes::Block>* m_buffer_queue_out;

	TempFileManager* mp_temp_file_manager;
	SortReduceUtils::MutexedQueue<SortReduceTypes::File>* mq_temp_files;

	size_t m_buffer_size;
	int m_buffer_count;


	void** mpp_managed_buffers;
	std::stack<int> ms_free_managed_buffers;

	SortReduceTypes::ComponentStatus m_status;

	SortReduceTypes::Config<K,V>* mp_config;
};

#endif
