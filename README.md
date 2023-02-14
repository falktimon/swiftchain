# swiftchain
### A library for building performant Blockchain applications in Python

**Please note: swiftchain is currently still in early development and not yet in a production-ready state.**

This library provides an easy-to-use, mid-level API for building Blockchain applications in Python. It allows (truly) parallelized mining of Blocks on consumer CPUs, implements the Nakamoto consensus algorithm and exposes an additional Node class, which maybe used as a basis for implementing a network node in a Peer-To-Peer network.

## Installation

swiftchain may be installed via pip using the following command:
```
pip install swiftchain
```
Even though this is the prefered method for installing swiftchain, it is also possible to build the library from source. Doing to requires a current version of GCC. Follow these simple steps:

```
# Clone the repository:
git clone https://github.com/mnt-home/swiftchain.git
# Change into source directory:
cd swiftchain/src
# Run the setup script:
./make_lib
```

This will create an object file for your platform, which may be included in a project locally or added to the ```site-packages``` folder in the Python installation directory.


## Limitations

Currently, a few specific things are hardcoded into the library and cannot be changed. These limitations mostly apply to the prebuild Blockchain class.

* The findConsensus() method will always run the Nakamoto consensus on a given ledger. The ability to define a custom algorithm for finding a consensus has not yet been implemented.
* swiftchain only exposes classes and methods for building blockchains - networking must be implemented *ad hoc*. Supporting networking is currently not a goal in swiftchain development.
* swiftchain has currently only been tested on a Linux machine and may not work as expected on a different OS
* Currently, it is not possible to mine blocks with an infinite number of tries/hashes. This will, however, be implemented in a coming version.

## Known Issues

* When mining two Blocks with the same content and node address, verification on the chain sometimes fails. This happens only if they are also mined with the same nonce and in the same millisecond. The odds of this happening decrease over time.
