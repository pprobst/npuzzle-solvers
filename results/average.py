#!/usr/bin/env python3
"""
Use as
./results.py <file.csv>
"""

import sys
import pandas as pd

pd.set_option('display.float_format', lambda x: '%.6f' % x)

f = pd.read_csv(sys.argv[1])
avg = f.mean(axis=0)
print(avg)
