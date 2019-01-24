import numpy as np
import sys

assert(len(sys.argv) == 2)

with open(sys.argv[1], "r") as fin:
    l = np.array([int(l) for l in fin])
    m, s = np.mean(l), np.std(l)
    u, l = np.max(l), np.min(l)
    print("%g %g %g" % (m, s/m, u-l))
