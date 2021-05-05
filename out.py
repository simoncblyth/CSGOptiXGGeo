#!/usr/bin/env python
"""

   CSGOptiXGGeo > out.txt
   grep "// bounds" out.txt  > bounds.txt

// bounds identity 1 primIdx 2741 nodeOffset 15879 numNode 1 aabb    -42.500    -42.500   -903.800       42.500     42.500    903.800 


In [10]: primIdx.min()
Out[10]: 0

In [11]: primIdx.max()
Out[11]: 3039

In [12]: np.unique(primIdx)
Out[12]: array([   0,    1,    2, ..., 3037, 3038, 3039])

"""

import numpy as np

o = np.loadtxt("bounds.txt", dtype=np.object)

primIdx = o[:,5].astype(np.int)



