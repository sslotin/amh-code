compile = g++ -std=c++17 -O3 -march=native -funroll-loops -Wall

%: %.cc argmin.hh
	$(compile) $< -o $@ 

%-dec: %.cc argmin.hh
	$(compile) -D DEC $< -o $@ 

%.s: %.cc argmin.hh
	$(compile) -S -masm=intel -fverbose-asm -funroll-loops $< -o $@

%.run: %
	@./$<

.PHONY: %.run
