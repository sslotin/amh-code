import time
import random

def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)

n = 100
k = 1000

def num():
    return random.randint(1, 10**9)

a = [num() for _ in range(k)]
b = [num() for _ in range(k)]

start = time.time()

for _ in range(n):
    for i in range(k):
        gcd(a[i], b[i])

print((time.time() - start) / n / k * 10 ** 9)

