CXXFLAGS := -g -O0

all: main main1

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

run:
	./main  100000000
	./main1 100000000

submit:
	qsub jobfile