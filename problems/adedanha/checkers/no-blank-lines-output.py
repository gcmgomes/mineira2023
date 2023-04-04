#!/usr/bin/env python2.7
# encoding: utf-8

import sys

for line in sys.stdin.xreadlines():
    if not line.strip():
        sys.exit(1)
