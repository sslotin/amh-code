compile = g++ -std=c++17 -O3 -ffast-math -march=native -funroll-loops -Wall

%: %.cc fft.hh
	$(compile) $< -o $@ 

%.s: %.cc fft.hh
	$(compile) -S -masm=intel -fverbose-asm -funroll-loops $< -o $@

%.run: %
	@./$<

.PHONY: %.run
