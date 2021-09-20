COMPILE = g++ -std=c++17 -O3 -ffast-math -funroll-loops -march=native -Wall

SOLUTIONS = $(wildcard v*.cc)

n ?= 256
s ?= 1

all: $(addsuffix .benchmark.result,$(basename $(SOLUTIONS)))
	for number in 1 2 3 4 ; do \
	    echo $$number ; \
	done
	for file in $^; do \
		res=`cat $$file`; \
		echo "$(basename $$file): $$res"; \
	done

%.test: %.cc test.cc matmul.hh
	$(COMPILE) $< test.cc -o $@ 

%.benchmark: %.cc benchmark.cc matmul.hh
	$(COMPILE) $< benchmark.cc -o $@

%.benchmark.result: %.benchmark
	@# ./$< $(n) > $@
	@./$< $(n) $(s)

.PHONY: %.benchmark.result
