#!/usr/bin/env python3
# encoding: utf-8

# Sample test generator

import sys
from random import seed, randint

# sets up seed to ensure that test cases are generated deterministically
# based on the input arguments
seed('|'.join(sys.argv[1:]))

primes = [100001303, 100001537, 100000627, 100000609, 999999733, 100000049, 100000717, 999997577, 100003301, 100002499]

print(randint(1, 5000),primes[randint(0,9)])
