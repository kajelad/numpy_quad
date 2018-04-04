import numpy as np

from npquad.numpy_quad import quad
# from npquad.info import __doc__

__all__ = ['quad']

if np.__dict__.get('quad') is not None:
    raise RuntimeError('The NumPy package already has a quad type')

np.quad = quad
np.typeDict['quad'] = np.dtype(quad)
