# swiftchain
### A tiny, opinionated library for building performant Blockchain applications in Python

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

## Basic Usage

```python
from swiftchain import Blockchain, Node

# Create a Node with some user name.
# The hash generated from this user name is used as the Node address:
user = Node("Some user name to be hashed")

# Create a blockchain with a difficulty threshold of 7, 
# meaning that the difficulty is increased every 7 blocks:
chain = Blockchain(diff_threshold=7, g_data="Fiat Lux!",
                   node_addr=user.get_node_addr())

# Mine 100 blocks on the above chain.
# This will take ~8 seconds on an average consumer PC:
for i in range(100):
    user.write_data(data="Hello " + str(i), chain=chain)

# Print the content of the last 5 blocks onto the screen:
print(user.read_data_by_range(range=5, chain=chain))

# Output: ['Hello 95', 'Hello 96', 'Hello 97', 'Hello 98', 'Hello 99']

```


## Limitations

Currently, a few specific things are hardcoded into the library and cannot be changed. These limitations mostly apply to the prebuild Blockchain class.

* swiftchain has currently only been tested on a Linux machine and may not work as expected on a different OS
* The API does not include methods for networking - these have to be implemented *ad-hoc*. This may change in a future version.

## Features

* Exposes basic building blocks of Blockchain applications in an easy-to-use API
* Permissive open-source license (Apache-2.0)
* Tiny codebase (~1400 SLoC, excluding pybind11 and tests) with a small installation size (500KB)

## Known Issues

* When mining two Blocks with the same content and node address, verification on the chain sometimes fails. This happens only if they are also mined with the same nonce and in the same millisecond. The odds of this happening decrease over time.
