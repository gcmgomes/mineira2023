#!/usr/bin/env python3
# encoding: utf-8

# Sample test generator

import sys, math
from random import seed, randint

# sets up seed to ensure that test cases are generated deterministically
# based on the input arguments
seed(sys.argv[1])

n=int(sys.argv[2])
m=int(sys.argv[3])
max_a=int(sys.argv[4])

print(n, m)

rint = [randint(0, max_a) for _ in range(2)]

for i in range(n):
    for j in range(m):
        if randint(1, 1e4) == 1e4:
            print(rint[0], end='')
        else:
            print(rint[1], end='')
            
        if j < m - 1:
            print(' ', end='')
        else:
            print()