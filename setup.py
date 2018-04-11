from setuptools import setup, Extension
import numpy as np

ext_modules = []

ext = Extension('npquad.numpy_quad',
                sources=['npquad/quad.c',
                         'npquad/numpy_quad.c'],
                include_dirs=[np.get_include()],
                libraries=['quadmath'],
                extra_link_args=['-lquadmath'])
ext_modules.append(ext)

setup(name='npquad',
      version='0.1',
      description='NumPy IEEE quadruple precision floating point',
      packages=['npquad'],
      ext_modules=ext_modules)
