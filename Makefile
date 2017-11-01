CXXFLAGS := -g -O0

all: main

%: %.cpp
	mpicc $(CXXFLAGS) $< -o $@ -lmpi