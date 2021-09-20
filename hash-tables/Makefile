compile = g++ -std=c++17 -O3 -march=native -Wall

%: %.cc hashmap.hh
	$(compile) $< -o $@ 

%.s: %.cc hashmap.hh
	$(compile) -S -fverbose-asm $< -o $@

%.run: %
	stdbuf --output=L ./$< | tee $@
