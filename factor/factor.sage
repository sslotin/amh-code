from time import time

with open('test.in', 'r') as f:
    cnt = 0
    err = 0
    start = time()

    for l in f:
        n = int(l)
        f = factor(n)[0][0]
        err += (f == 1 or f == n)
        cnt += 1

    print(cnt / (time() - start), f'{err}/{cnt}')
