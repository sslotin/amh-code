compile = g++ -std=c++17 -O3 -march=native -funroll-loops -Wall

%: %.cc binpow.hh
	$(compile) $< -o $@ 

%.s: %.cc binpow.hh
	$(compile) -S -masm=intel -fverbose-asm $< -o $@

%.run: %
	@./$<

.PHONY: %.run
