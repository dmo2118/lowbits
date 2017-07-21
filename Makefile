# GCC 5 and earlier needs -std=c++14.
CXXFLAGS=-std=c++14 -Wall -g # -O3 -DNDEBUG

exes=test_simple test_permute test_time
objs=$(patsubst %,%.o,$(exes))

compile_cmd=$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o

all: $(exes)

clean:
	rm -f $(exes) $(objs) lowbits.o

%: %.o lowbits.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

%.o: %.cpp test.hpp lowbits.hpp
	$(compile_cmd) $@ $<

lowbits.o: lowbits.cpp lowbits.hpp
	$(compile_cmd) $@ $<

.SECONDARY: $(objs)

# Inhibit implicit straight-from-source build rule
%: %.cpp
