compile = clang++ -std=c++17 -O3 -march=native

%: %.cc binsearch.hh Makefile
	$(compile) $< -o $@ 

%.s: %.cc binsearch.hh Makefile
	$(compile) -S -masm=intel -fverbose-asm $< -o $@

%.run: %
	@./$<

.PHONY: %.run
