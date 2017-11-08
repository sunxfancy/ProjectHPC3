CXXFLAGS := -g -O0

all: main main1 main2 main3

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

.PHONY: run submit clean
run:
	./main  10000000000
	./main1 10000000000
	./main2 10000000000
	./main3 10000000000

submit:
	qsub jobfile

clean:
	rm -f XiaofanHPC3.* main main1 main2 main3