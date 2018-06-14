import numpy as np

from npquad.numpy_quad import quad
# from npquad.info import __doc__

__all__ = ['quad']

if np.__dict__.get('quad') is not None:
    raise RuntimeError('The NumPy package already has a quad type')

np.quad = quad
np.typeDict['quad'] = np.dtype(quad)


def add_to_finfo():
    # Inject float16 into the finfo cache
    # by hijacking the normal construction method
    fi = object.__new__(np.finfo)
    fi.dtype = np.dtype(quad)

    fi.machar = None

    fi.eps = quad("1.92592994438723585305597794258492732e-34")
    fi.epsneg = quad("9.62964972193617926527988971292463659e-35")
    fi.iexp = 15
    fi.machep = -112
    fi.max = quad("1.18973149535723176508575932662800702e+4932")
    fi.maxexp = 16384
    fi.min = quad("-1.18973149535723176508575932662800702e+4932")
    fi.minexp = -16382
    fi.negep = -113
    fi.nexp = 15
    fi.nmant = 112
    fi.precision = 33
    fi.resolution = quad("1e-33")
    fi.tiny = quad("3.3621031431120935062626778173217526e-4932")

    fi._str_eps = "1.92592994438723585305597794258492732e-34"
    fi._str_epsneg = "9.62964972193617926527988971292463659e-35"
    fi._str_resolution = "1.00000000000000000000000000000000005e-33"
    fi._str_max = "1.18973149535723176508575932662800702e+4932"
    fi._str_tiny = "3.3621031431120935062626778173217526e-4932"

    np.finfo._finfo_cache[fi.dtype] = fi

add_to_finfo()
