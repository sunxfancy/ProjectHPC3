CXXFLAGS := -O3 -Wall -DNDEBUG

all: main main1 main2 main3

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 

clean:
	rm -f XiaofanHPC3* main main1 main2 main3 result

.PHONY: clean