compile = g++ -std=c++17 -O3 -march=native -funroll-loops -Wall

n ?= 256
s ?= 1

%: %.cc floyd.hh
	$(compile) $< -o $@ 

%.s: %.cc floyd.hh
	$(compile) -S -masm=intel -fverbose-asm -funroll-loops $< -o $@

%.run: %
	@./$< $(n) $(s)

.PHONY: %.run
