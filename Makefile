CXXFLAGS := -g -O0

all: main main1 main2 main3

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

.PHONY: submit clean

submit:
	qsub jobfile

clean:
	rm -f XiaofanHPC3.* main main1 main2 main3