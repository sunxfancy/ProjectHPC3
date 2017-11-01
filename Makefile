CXXFLAGS := -g -O0

all: main

%: %.cpp
	mpic++ $(CXXFLAGS) $< -o $@ -lmpi