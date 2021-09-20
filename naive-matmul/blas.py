import os
os.environ["OMP_NUM_THREADS"] = "1"
# ^ making sure numpy won't cheat by using more than one core

import time
import numpy as np

n = 1024

a = np.random.rand(n, n)
b = np.random.rand(n, n)

start = time.time()

cnt = 1

for _ in range(cnt):
    c = np.dot(a, b)

duration = time.time() - start
print(duration / cnt)
