CXXFLAGS := -g -O0

all: main

%: %.cpp
	g++ $(CXXFLAGS) $< -o $@ -lmpi