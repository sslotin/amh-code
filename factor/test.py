p = random_prime(2^32, lbound=2^31)
q = random_prime(2^32, lbound=2^31)

out = !./factor {p * q}