CXXFLAGS := -O3 -Wall -DNDEBUG

all: load compile submit

compile: main main1 main2 main3

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

load:
	sh load_env.sh

submit: 
	qsub jobfile
	qsub jobfile2
	qsub jobfile4
	qsub jobfile8

clean:
	rm -f XiaofanHPC3* main main1 main2 main3

.PHONY: submit clean load compile all