#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include <tuple>
#include <fstream>

#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"

#include "EdgeProcess.h"
#include "VertexValues.h"

// typedef uint32_t wd_t;
typedef uint32_t wd_t; // walk data type

// //merge walks of a same vertex
// inline wd_t vertex_update(wd_t a, wd_t b) { // a+b
// 	printf( "Vertex_update a: %d b: %d\n", a, b);
// 	wd_t ret = a + b;
// 	return ret;
// }

size_t g_vertex_count = 0;
inline wd_t edge_program(uint32_t vid, wd_t value, uint32_t fanout) { // value/fanout
	
	wd_t ret;
	if(fanout >0){
		ret = (wd_t)rand()%fanout;
	}else{
		ret = (wd_t)((value >> 14) & 0x3ffff);//sources
	}
	// printf( "Edge-program vid: %d, outd: %d, walk%d forward to %dth neighbor. \n", vid, fanout, value, ret);

	return ret;
}

inline wd_t finalize_program(wd_t oldval, wd_t val) { /// val*0.85 + 0.15/|V|
	return val;
}

inline bool is_active(wd_t old, wd_t newv, bool marked) { // |newv-old| > 0.0001
	if ( !marked ) return false;

	if(newv !=  0) return true;
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

	int a = atoi(argv[4]);
	int b = atoi(argv[5]);
	int R = 1600;
	int L = 11;

	int max_thread_count = 12;
	int max_sr_thread_count = 8;
	int max_vertexval_thread_count = 4;
	int max_edgeproc_thread_count = 8;
	if ( argc > 6 ) {
		max_thread_count = atoi(argv[6]);
	}
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


	EdgeProcess<uint32_t,wd_t>* edge_process = new EdgeProcess<uint32_t,wd_t>(idx_path, mat_path, &edge_program);
	size_t vertex_count = edge_process->GetVertexCount();
	printf( "vertex_count = %d \n", vertex_count );
	g_vertex_count = vertex_count;
	VertexValues<uint32_t,wd_t>* vertex_values = new VertexValues<uint32_t,wd_t>(tmp_dir, vertex_count, 0, &is_active, &finalize_program, max_vertexval_thread_count);



	int iteration = 0;
	while ( true ) {

		SortReduceTypes::Config<uint32_t,wd_t>* conf =
			new SortReduceTypes::Config<uint32_t,wd_t>(tmp_dir, "", max_sr_thread_count);
		conf->quiet = true;
		// conf->SetUpdateFunction(&vertex_update);
		// conf->SetUpdateFunction(NULL);

		SortReduce<uint32_t,wd_t>* sr = new SortReduce<uint32_t,wd_t>(conf);
		SortReduce<uint32_t,wd_t>::IoEndpoint* ep = sr->GetEndpoint(true);
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
				wd_t w = ( (a & 0x3ffff) << 14 ) | (0 & 0x3fff);
				edge_process->SourceVertex(a, w, true);
			}
			for ( size_t i = 0; i < R; i++ ) {
				wd_t w = ( (b & 0x3ffff) << 14 ) | (0 & 0x3fff);
				edge_process->SourceVertex(b, w, true);
			}
		} else {
			int fd = vertex_values->OpenActiveFile(iteration-1);
			SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(fd);
			std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
			while ( std::get<2>(res) ) {
				uint32_t key = std::get<0>(res);
				wd_t val = std::get<1>(res);
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
		std::tuple<uint32_t,wd_t,bool> res = sr->Next();
		vertex_values->Start();
		while ( std::get<2>(res) ) {
			uint32_t key = std::get<0>(res);
			wd_t val = std::get<1>(res);

			// printf( "\t\t++ SRR vertex %d : %d walks.\n", key, val );
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
	// printf( "\t\t++ All Done! %lu ms\n", duration_milli.count() );
	printf( "\t\t++ All Done! %f s\n", duration_milli.count()/1000.0 );

	std::string statistic_filename = "grafsoft.statistics";
	std::ofstream ofs;
	ofs.open(statistic_filename.c_str(), std::ofstream::out | std::ofstream::app );
	ofs << duration_milli.count()/1000.0 << std::endl;
	ofs.close();

	exit(0);
}
