#!/usr/bin/env python3
# encoding: utf-8

# Sample test generator

import sys
from random import seed, randint

# sets up seed to ensure that test cases are generated deterministically
# based on the input arguments
seed('|'.join(sys.argv[1:]))

n = int(sys.argv[2])
print(n)

for i in range(n):
    print(randint(1, 3), randint(1, 3))
