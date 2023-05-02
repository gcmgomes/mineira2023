#!/usr/bin/env python3
# encoding: utf-8

# Sample test generator

import sys
from random import seed, randint

# sets up seed to ensure that test cases are generated deterministically
# based on the input arguments
seed('|'.join(sys.argv[1:]))

N=int(sys.argv[1])
T=int(sys.argv[2])

print(N, T)

for i in range(N):
    k = randint(2, 100)
    if k%2 == 1:
        k -= 1

    print(k, randint(1, T), randint(1, 1e9))

for i in range(T):
    print(randint(0, 50000), randint(0, 50000))