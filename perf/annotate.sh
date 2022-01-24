g++ -O2 -g sort.cc -o run
valgrind --tool=cachegrind --branch-sim=yes --cachegrind-out-file=cachegrind.out ./run
cg_annotate cachegrind.out --auto=yes
