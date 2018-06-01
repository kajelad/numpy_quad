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

Designed for use in Linus-x86-64 architecture with gcc-5+. Untested on other
platforms and inlikely to be functional.

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

Basic features
	- scalar type initialization

Array implementation
	- 1d, nd arrays

Array algorithms
	- zeros
	- fill

Interation with other numpy types
	- casting to other simple numercal types
	- no.float128 interaction unsupported

Arithmetic
	- same type +-*/^
	- quad-double arithmetic lifted to quad precision

Special functions

-------------------------------------------------------------------------------
Issues

- dotfunc currently bugged, np.dot and the @ operator do not function.

- cannablizes the dtype descriptor of the np.float128 dtype. It is unclear what
effects this has on np.float128, using it in conjunction with np.quad is not
recommended (as a general consideration, using np.float128 in any other context
is also highly discouraged, it is poorly implemented, poorly maintained, and
highly platform specific).

- passing quads into the constructors for other numerical types may cause
memory issues. Using the quad.astype method is functional and preferred, even
for scalar values
