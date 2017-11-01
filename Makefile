CXXFLAGS := -g -O0

all: main

%: %.c
	mpicc $(CXXFLAGS) $< -o $@ 