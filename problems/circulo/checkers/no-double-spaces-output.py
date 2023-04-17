#!/usr/bin/env python2.7
# encoding: utf-8

import sys

for line in sys.stdin.xreadlines():
    if '  ' in line:
        sys.exit(1)
