#!/usr/bin/env python3

# Sample test generator

import sys,os,string
from random import seed, randint, sample

# first argument must be seed
seed(sys.argv[1])

print(sys.argv[2])
