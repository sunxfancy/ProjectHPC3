CXXFLAGS := -O3 -Wall -DNDEBUG

all: main main1 main2 main3

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

.PHONY: submit clean

submit:
	qsub jobfile
	qsub jobfile2
	qsub jobfile4
	qsub jobfile8

clean:
	rm -f XiaofanHPC3* main main1 main2 main3