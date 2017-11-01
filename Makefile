CXXFLAGS := -g -O0

all: main

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

run:
	./main 100000

submit:
	qsub jobfile