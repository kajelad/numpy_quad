Numpy_Quad

Numpy compatible 128-bit IEEE Floating Point numerical type
2018 Jack Dale

-------------------------------------------------------------------------------
Licensing

Individual source files derived from the numpy-dtypes repository, and retain
copyright information. Modification and distribution of these files are subject
to the restrictions therein.

-------------------------------------------------------------------------------
Compatibility

Designed for use in Linux-x86-64 architecture with gcc-5+. Untested on other
platforms.

-------------------------------------------------------------------------------
Installation

Running the setup script requires the setuptools python module. Due to ongoing
modifications, develop mode is recommended. To install from the command line,
run:

	sudo python3 setup.py develop
	sudo python3 setup.py clean   #optional

Optionally, if root access is unavailable, run:

	python3 setup.py develop --user

In this case, the base path to the repository may have to be added to
PYTHONPATH manually.

To uninstall:

	sudo python3 setup.py develop --uninstall

Once installed, the module will automatically be linked to a pythonpath-
accessable directory and ready to import.

Any files from the previous distutils based installation pattern will have to
be removed manually.

-------------------------------------------------------------------------------
Use

To add the quad type to numpy, simply import the module. The quad type will be
appended to numpy's type dict automatically as np.quad

    >>> import numpy as np
    >>> import npquad
    >>> ar = np.zeros((4, 4), dtype=np.quad)

-------------------------------------------------------------------------------
Features

Objects:
- scalar, array, ndarray

Functionality:

- initialization from strings via constructor

- upcasting via astype

- all array indexing

- scalar/array arithmetic, basic ndarray methods

- Interation with other numpy types
    - quad-double arithmetic lifted to quad precision
    - casting to/from other simple numercal types
    - np.float128 unsupported

- Some pecial functions

Visual:

- String outputs at 36 decimal digits of precision for invertible conversion
to/from strings. trailing digits are not within binary resolution.

-------------------------------------------------------------------------------
Issues

- cannablizes the dtype descriptor of the np.float128 dtype. It is unclear what
effects this has on np.float128; using it in conjunction with np.quad is even
less recommended than usual.

- passing quads into the constructors for other numerical types is
unimplemented and may cause memory issues. Using the quad.astype method is
functional and preferred, even for scalar values.

    q = np.quad("1.7")
    float_array = np.empty(16, dtype=np.float64)

    # Don't do these. They may result in a segfault
    float_array[0] = q
    f = float(q)

    # Instead use these patterns
    float_array[0] = q.astype(np.float64)
    f = q.astype(np.float64)
    
- NotImplementedError on various numpy functionality
    = matmul, einsum
