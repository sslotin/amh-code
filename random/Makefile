compile = g++ -std=c++17 -O3 -march=native  -fopt-info-vec-optimized -Wall

%: %.cc random.hh
	$(compile) -funroll-loops $< -o $@ 

%.s: %.cc random.hh
	$(compile) -S -fverbose-asm $< -o $@

%.run: %
	@./$<

.PHONY: %.run
