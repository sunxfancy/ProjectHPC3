CXXFLAGS := -g -O0

all: main main1 main2 main3

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

.PHONY: run submit clean
run:
	./main  100000000
	./main1 100000000
	./main2 100000000
	./main3 100000000

submit:
	qsub jobfile

clean:
	rm -f XiaofanHPC3.* main main1 main2 main3