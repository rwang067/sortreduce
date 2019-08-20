#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include <tuple>

#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"

#include "EdgeProcess.h"
#include "VertexValues.h"

typedef uint32_t wid_t;

//merge walks of a same vertex
inline wid_t vertex_update(wid_t a, wid_t b) { // a+b
	printf( "Vertex_update a: %d b: %d\n", a, b);
	wid_t ret = a + b;
	return ret;
}
inline wid_t edge_program(uint32_t vid, wid_t value, uint32_t fanout) { // value/fanout
	
	wid_t ret = rand()%fanout;
	// printf( "Edge-program vid: %d, outd: %d, walk%d forward to %dth neighbor. \n", vid, fanout, value, ret);

	return ret;
}

size_t g_vertex_count = 1;
inline wid_t finalize_program(wid_t oldval, wid_t val) { /// val*0.85 + 0.15/|V|
	uint32_t ret = val;
	return ret;
}

inline bool is_active(wid_t old, wid_t newv, bool marked) { // |newv-old| > 0.0001
	if ( !marked ) return false;

	if(newv > 0) return true;
	return false;
}

int main(int argc, char** argv) {
	srand(time(0));

	if ( argc < 6 ) {
		fprintf(stderr, "usage: %s directory ridx matrix nwalks nsteps.\n", argv[0] );
		exit(1);
	}

	char* tmp_dir = argv[1]; // directory of vertex value
	char* idx_path = argv[2]; // path of beg_pos file
	char* mat_path = argv[3]; // path of csr/csc file

	int R = atoi(argv[4]);
	int L = atoi(argv[5]);

	int max_thread_count = 12;
	int max_sr_thread_count = 8;
	int max_vertexval_thread_count = 4;
	int max_edgeproc_thread_count = 8;
	// if ( argc > 4 ) {
	// 	max_thread_count = atoi(argv[4]);
	// }
	if ( max_thread_count >= 32 ) {
		max_sr_thread_count = 28;
		max_vertexval_thread_count = 8;
		max_edgeproc_thread_count = 8;
	} else if ( max_thread_count >= 24 ) {
		max_sr_thread_count = 20;
		max_vertexval_thread_count = 8;
		max_edgeproc_thread_count = 8;
	}
		
	std::chrono::high_resolution_clock::time_point start_all;
	start_all = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point now;
	std::chrono::milliseconds duration_milli;

	//conf->SetManagedBufferSize(1024*1024*4, 256); // 4 GiB


	EdgeProcess<uint32_t,wid_t>* edge_process = new EdgeProcess<uint32_t,wid_t>(idx_path, mat_path, &edge_program);
	size_t vertex_count = edge_process->GetVertexCount();
	printf( "vertex_count = %d \n", vertex_count );
	g_vertex_count = vertex_count;
	VertexValues<uint32_t,wid_t>* vertex_values = new VertexValues<uint32_t,wid_t>(tmp_dir, vertex_count, 0, &is_active, &finalize_program, max_vertexval_thread_count);



	int iteration = 0;
	while ( true ) {

		SortReduceTypes::Config<uint32_t,wid_t>* conf =
			new SortReduceTypes::Config<uint32_t,wid_t>(tmp_dir, "", max_sr_thread_count);
		conf->quiet = true;
		conf->SetUpdateFunction(&vertex_update);

		SortReduce<uint32_t,wid_t>* sr = new SortReduce<uint32_t,wid_t>(conf);
		SortReduce<uint32_t,wid_t>::IoEndpoint* ep = sr->GetEndpoint(true);
		edge_process->SetSortReduceEndpoint(ep);
		for ( int i = 0; i < max_edgeproc_thread_count; i++ ) {
			//FIXME this is inefficient...
			edge_process->AddSortReduceEndpoint(sr->GetEndpoint(true));
		}

		std::chrono::high_resolution_clock::time_point start, iteration_start;
		start = std::chrono::high_resolution_clock::now();
		iteration_start = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds iteration_duration_milli;

		printf( "\t\t++ Starting iteration %d\n", iteration ); fflush(stdout);
		edge_process->Start();

		if ( iteration == 0 ) {
			for ( size_t i = 0; i < R; i++ ) {
				uint32_t v = rand()%vertex_count;
				edge_process->SourceVertex(v, 1, true);
			}
		} else {
			int fd = vertex_values->OpenActiveFile(iteration-1);
			SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(fd);
			std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
			while ( std::get<2>(res) ) {
				uint32_t key = std::get<0>(res);
				wid_t val = std::get<1>(res);
				if( val > 0 ){
					// printf( "Vertex %d %d\n", key, val );
					edge_process->SourceVertex(key, val, true);
				}	
				res = reader->Next();
			}
			delete reader;
		}
		edge_process->Finish();
		sr->Finish();
		now = std::chrono::high_resolution_clock::now();
		duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start);
		printf( "\t\t++ Input done : %lu ms\n", duration_milli.count() );

		start = std::chrono::high_resolution_clock::now();
		SortReduceTypes::Status status = sr->CheckStatus();
		while ( status.done_external == false ) {
			usleep(1000);
			status = sr->CheckStatus();
		}

		now = std::chrono::high_resolution_clock::now();
		duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start);
		printf( "\t\t++ Sort-Reduce done : %lu ms\n", duration_milli.count() );
		fflush(stdout);

		
		start = std::chrono::high_resolution_clock::now();
		std::tuple<uint32_t,wid_t,bool> res = sr->Next();
		vertex_values->Start();
		while ( std::get<2>(res) ) {
			uint32_t key = std::get<0>(res);
			wid_t val = std::get<1>(res);

			printf( "\t\t++ SRR vertex %d : %d walks.\n", key, val );
			while ( !vertex_values->Update(key,val) ) ;

			res = sr->Next();
		}


		vertex_values->Finish();
		size_t active_cnt = vertex_values->GetActiveCount();

		now = std::chrono::high_resolution_clock::now();
		duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start);
		iteration_duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-iteration_start);
		
		printf( "\t\t++ Iteration done : %lu ms / %lu ms, Active %ld\n", duration_milli.count(), iteration_duration_milli.count(), active_cnt );
		if ( active_cnt == 0 ) break;
		if ( iteration >= L-1 ) break;
		vertex_values->NextIteration();
		iteration++;

		delete sr;

	}
	now = std::chrono::high_resolution_clock::now();
	duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start_all);
	printf( "\t\t++ All Done! %lu ms\n", duration_milli.count() );

	exit(0);
}
