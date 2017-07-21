# GCC 5 and earlier needs -std=c++14.
CXXFLAGS=-std=c++14 -Wall -g # -O3 -DNDEBUG

exes=test_simple test_permute test_time

all: $(exes)

clean:
	-rm $(exes)

%: %.cpp lowbits.hpp test.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
