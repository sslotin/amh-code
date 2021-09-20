import fire
import numpy


def gen(n=2**24, fname='input.bin'):
    numpy.random.randint(10**9, size=n, dtype='int32').tofile(fname)

def read(fname='output.bin', l=0, r=10):
    return numpy.fromfile(fname, dtype='int32')[l:r]

def check(fname='output.bin'):
    return numpy.all(numpy.diff(read(fname)) >= 0)


fire.Fire()
