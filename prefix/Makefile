compile = g++ -std=c++17 -O3 -march=native -funroll-loops -Wall

n ?= 4096

%: %.cc prefix.hh
	$(compile) $< -o $@ 

%.run: %
	@./$< $(n)

.PHONY: %.run
