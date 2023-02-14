# swiftchain
### A tiny, opinionated library for building performant Blockchain applications in Python

**Please note: swiftchain is currently still in early development.**

This library provides an easy-to-use, mid-level API for building Blockchain applications in Python. It allows parallelized mining of Blocks on consumer CPUs, implements the Nakamoto consensus algorithm and exposes an additional Node class, which maybe used as a basis for implementing a network node in a Peer-To-Peer network.

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

Here is a basic example of creating a Blockchain and mining blocks:

```python
from swiftchain import Blockchain, Node

# Create a Node with some user name.
# The hash generated from this user name is used as the Node address:
user = Node("Some user name to be hashed")

# Create a blockchain with a difficulty threshold of 7, 
# meaning that the difficulty increases every 7 blocks:
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
This blockchain object may then be, for example, pickled into a byte representation and send to another node in a network.
For more information on how to use swiftchain, please consult the wiki.

## Limitations

There are some limitations which currently affect swiftchain:

* swiftchain has only been tested on a Linux machine and may not work as expected on a different OS
* The API does not include methods for networking - these have to be implemented *ad-hoc*. This may change in a future version.
