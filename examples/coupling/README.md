# Environmental conflict example

More information will be added here after publication.

## Installation

1. Download OpenWorld from https://github.com/jrising/openworld
2. Download OpenDims from https://github.com/jrising/opendims
3. Edit the makefile to point to the root directory of OpenWorld and OpenDims.

* Run `make` to compile `conflict.cpp`, which when run produces output for a series of aggregate tests.
* Run `make c2` to compile `conflict2.cpp`, which when run produces output for a distributed test.

To compile `conflict2.cpp`, you will need to install the GSL library:
http://www.gnu.org/software/gsl/
