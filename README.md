# swiftchain
### A tiny, opinionated library for building performant Blockchain applications in Python

**Please note: swiftchain is currently still in early development.**

This library provides an easy-to-use, mid-level API for building Blockchain applications in Python. It allows parallelized mining of Blocks on consumer CPUs, implements the Nakamoto consensus algorithm and exposes an additional Node class, which maybe used as a basis for implementing a network node in a Peer-To-Peer network.

## Installation

swiftchain may be installed via pip using the following command:
```
pip install swiftchain
```
```pip``` is the prefered method for installing swiftchain. If you're on a UNIX-like system however, it should also possible to build the library from source. Doing so requires a current version of ```c++``` from ```gcc```. Follow these simple steps:

```bash
# Clone the repository:
git clone https://github.com/mnt-home/swiftchain.git
# Change into source directory:
cd swiftchain/src
# Run the setup script:
./make_lib
```

This will create an object file for your platform, which may be included in a project locally or added to the ```site-packages``` folder in the Python installation directory.

## Basic Usage

Here is a basic example of creating a Blockchain and mining blocks:

```python
from swiftchain import Blockchain, Node

# Create a Node with some user name.
# The hash generated from this user name is used as the Node address:
user = Node("Some user name to be hashed")

# Create a blockchain with a difficulty threshold of 7,
# meaning that the difficulty is increased every 7 blocks.
chain = Blockchain(diff_threshold=7, g_data="Fiat Lux!",
                   node_addr=user.get_node_addr())

# Mine 100 blocks on the above chain.
# The amount of time this takes will vary depending on your system,
# with some degree of luck thrown in.
for i in range(100):
    user.write_data(data="Hello " + str(i), chain=chain)

# Print the content of the last 5 blocks onto the screen:
print(user.read_data_by_range(5, chain=chain))

# Output: ['Hello 95', 'Hello 96', 'Hello 97', 'Hello 98', 'Hello 99']

```
By default, this example uses 5 threads to mine a block on the blockchain. Feel free to vary the amount of threads by specifiying the optional ```threads``` parameter of the ```write_data``` function, to see how it impacts performance.
For more details on how to use this library, please consult the [wiki](https://github.com/mnt-home/swiftchain/wiki).

## Limitations

There are some limitations which currently affect swiftchain:

* swiftchain has only been tested on a Linux machine and may not work as expected on a different OS. It doesn't, however, contain any platform-specific code.
* Currently, swiftchain is probably not very scalable, as it is not really optimized. This hasn't been tested yet properly though and may thus turn out to not be a problem.
* The API does not include methods for networking - these have to be implemented *ad-hoc*. This may change in a future version.

## Dependencies

The only dependency, which is only used for building the library, is pybind11. It's automatically included by ```pip```, so there's usually no need to take action. If you're building this library from source for some reason, there are no dependencies as pybind11 is included as a source directory. 

