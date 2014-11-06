#!/usr/bin/python

import sys
from pylab import *

fl = open(sys.argv[1], "r")
lnr = fl.readlines()[1 : -1]
fl.close()

fn = lambda x: float(
        x.rsplit()
        [0]
        .replace(",", ".")
        )

ln = list(map(fn, lnr))
num = len(ln)

tm = list(map(lambda x: x * 0.005, range(num)))

plot(tm, ln)
grid(True)
show()

