# GCC 5 and earlier needs -std=c++14.
CXXFLAGS=-std=c++14 -Wall -g # -O3 -DNDEBUG

exes=lowbits1 test_perm test_time

all: $(exes)

clean:
	-rm $(exes)

%: %.cpp lowbits.hpp lowbits2.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
