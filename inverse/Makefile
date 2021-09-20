compile = g++ -std=c++17 -O3 -march=native -funroll-loops -Wall

%: %.cc inverse.hh
	$(compile) $< -o $@ 

%.s: %.cc inverse.hh
	$(compile) -S -masm=intel -fverbose-asm $< -o $@

%.run: %
	@./$<

.PHONY: %.run
