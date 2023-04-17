#!/usr/bin/env python3

# Sample test generator

import sys
import os
import string
from random import seed, randint, sample

# first argument must be seed
seed(sys.argv[1])
n = int(sys.argv[2])
cur_color = 1
ranges = [[0, n-1]]
cris = [0 for x in range(0, n)]
while len(ranges) > 0:
    r = ranges.pop(0)
    if r[0] == r[1]:
        cris[r[0]] = cur_color
    else:
        c = randint(1, min(3, r[1] - r[0] + 1))
        points = sample(range(r[0], r[1]+1), c)
        points.sort()
        # print(points)
        for q in points:
            cris[q] = cur_color
        if r[0] != points[0]:
            ranges.append([r[0], points[0]-1])
        for i in range(0, len(points)-1):
            if points[i] + 1 <= points[i+1] - 1:
                ranges.append([points[i]+1, points[i+1] - 1])
        if points[-1] != r[1]:
            ranges.append([points[-1]+1, r[1]])
    cur_color += 1
print(n)
print(*cris)
