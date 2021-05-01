from math import log
from fire import Fire

def f(n):
    mergesort = n * log(n, 2)
    quicksort = sum([
        1 / abs(i-j)
        for i in range(n)
        for j in range(n)
        if i != j
    ])
    return mergesort, quicksort

Fire(f)

