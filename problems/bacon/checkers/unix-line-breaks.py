#!/usr/bin/env python2.7
# encoding: utf-8

import sys

for line in sys.stdin.xreadlines():
    if line.endswith('\n\r') or line.endswith('\r\n'):
        sys.exit(1)
    if not line.endswith('\n'):
        sys.exit(1)
