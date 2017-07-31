# GCC 5 and earlier needs -std=c++14.
CXXFLAGS=-std=c++11 -Wall -g # -O3 -DNDEBUG

exes=test_simple test_permute test_time example
objs=$(patsubst %,%.o,$(exes))

compile_cmd=$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o

all: $(exes)

clean:
	rm -f $(exes) $(objs) lowbits.o # $(png)

%: %.o lowbits.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

%.o: %.cpp test.hpp lowbits.hpp
	$(compile_cmd) $@ $<

lowbits.o: lowbits.cpp lowbits.hpp
	$(compile_cmd) $@ $<

example.o: example.cpp lowbits.hpp
	$(compile_cmd) $@ $<

test: test_simple test_permute example
	./test_simple
	tmp=`mktemp` && ./example > $$tmp && (echo '0 1 2 3 4 5 6 7 ' | diff - $$tmp) && rm $$tmp
	./test_permute 10

# tree*.png files are for the benefit of GitHub, which does not support SVG images.
png=$(patsubst %.svg,%.png,$(wildcard tree*.svg))

png: $(png)

%.png: %.svg style.css
	rsvg-convert -o $@ $<
	-optipng -quiet -o5 $@

.SECONDARY: $(objs)

# Inhibit implicit straight-from-source build rule
%: %.cpp
