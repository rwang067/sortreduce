# TARGETS=bfs pr bc 
# TARGETS=randomwalks/ppr randomwalks/rwd randomwalks/gl randomwalks/sr
TARGETS=randomwalks/rawrandomwalks randomwalks/rwd randomwalks/ppr randomwalks/sr randomwalks/gl

SORTREDUCE_INCLUDE=../../include/
SORTREDUCE_LIB=../../obj/

GRAPH_SRC=src/
GRAPH_CPP=$(wildcard $(GRAPH_SRC)/*.cpp)

SRFLAGS=-DKVTYPES1='uint32_t,uint32_t' -DKVTYPES2='uint64_t,uint32_t'  
# SRFLAGS=-DKVTYPES1='uint32_t,uint32_t' -DKVTYPES2='uint32_t,uint64_t'  -DKVTYPES3='uint64_t,uint32_t'  -DKVTYPES4='uint64_t,uint64_t' 


OUTPUTDIR=./obj
OBJ=$(patsubst %,$(OUTPUTDIR)/%, $(TARGETS))
all: $(OBJ)


$(OUTPUTDIR)/%: %.cpp $(SORTREDUCE_LIB)/libsortreduce.a $(GRAPH_CPP) | $(OUTPUTDIR)
	g++ -o $(OUTPUTDIR)/$(basename $<) $< $(GRAPH_CPP) $(SRFLAGS) -w -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -pedantic -lsortreduce -pthread -laio -march=native -lrt -O2 -g
	# g++ -o $(OUTPUTDIR)/$(basename $<) $< $(GRAPH_CPP) $(SRFLAGS) -Wall -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -pedantic -lsortreduce -pthread -laio -march=native -lrt -O2 -g
	# g++ -o $(OUTPUTDIR)/$(basename $<) $< $(GRAPH_CPP) $(SRFLAGS) -Wall -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -pedantic -lsortreduce -pthread -laio -march=native -lrt -O0 -g

$(OUTPUTDIR):
	mkdir -p obj
	mkdir -p obj/randomwalks

clean:
	rm -rf ./obj/
	

#obj/pr: pr.cpp $(SORTREDUCE_LIB)/libsortreduce.a $(GRAPH_CPP)
#	mkdir -p obj
#	g++ -o ./obj/pr pr.cpp $(GRAPH_CPP) $(SRFLAGS) -Wall -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -pedantic -lsortreduce -pthread -laio -march=native -lrt -g
