#!/usr/bin/env python2.7
# encoding: utf-8

import sys

for line in sys.stdin.xreadlines():
    if line.endswith('\n'):
        line = line[:-1]
    if line.rstrip() != line:
        sys.exit(1)
