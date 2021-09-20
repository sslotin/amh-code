compile = g++ -std=c++17 -O3 -funroll-loops -march=native -Wall

%: %.cc prefix.hh
	$(compile) $< -o $@ 

%.s: %.cc prefix.hh
	$(compile) -S -masm=intel -fverbose-asm $< -o $@

%.run: %
	@./$<

.PHONY: %.run
