from time import time

with open('test.in', 'r') as f:
    cnt = 0
    start = time()

    for l in f:
        n = int(l)
        assert(len(factor(n)) > 1)
        cnt += 1

    print(cnt / (time() - start))
