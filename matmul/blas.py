import os
os.environ["OMP_NUM_THREADS"] = "1" # export OMP_NUM_THREADS=4
os.environ["OPENBLAS_NUM_THREADS"] = "1" # export OPENBLAS_NUM_THREADS=4 
os.environ["MKL_NUM_THREADS"] = "1" # export MKL_NUM_THREADS=6
os.environ["VECLIB_MAXIMUM_THREADS"] = "1" # export VECLIB_MAXIMUM_THREADS=4
os.environ["NUMEXPR_NUM_THREADS"] = "1" # export NUMEXPR_NUM_THREADS=6

import glob
import time
import numpy as np

from fire import Fire

def matmul(n, testing_duration=1):
    a = np.random.rand(n, n).astype('float32')
    b = np.random.rand(n, n).astype('float32')
    start = time.time()
    cnt = 0
    while time.time() - start < testing_duration:
        c = np.dot(a, b)
        cnt += 1
    duration = (time.time() - start) / cnt
    print(f'{duration:6f} x {cnt}')
    return duration

Fire(matmul)
