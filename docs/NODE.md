
---
# Index

- [Node Class](#node-class)
  * [Node Constructor](#node-constructor)
  * [Node API](#node-api)
    + [Reading/Writing Methods](#reading-writing-methods)
  * [Ledger Access Methods](#ledger-access-methods)
  * [Getters and Setters](#getters-and-setters)
    + [Setters](#setters)
    + [Getters](#getters)
---

# Node Class

An object of a Node class represents a Blockchain user, in the broadest sense.
It may supply a base for e.g. a node in a P2P network, or it may represent a local user with access rights for a
relevant Blockchain. More specifically, this class mostly exposes higher-level wrappers over methods from the Blockchain classes. Once again, all of these methods can be called with keyword arguments.
This class exposes the following methods:

## Node Constructor

The Node constructor is used to instantiate a new Node object. The constructor takes one argument, which is the name of the Node.

**Method Signature:**

```python
Node(node_name);
```

**Parameters:**

* ```node_name```: A name associated with this Node object.

A hash is then generated from this Node name, which can be used to identify nodes in e.g. a network, grand access rights to a particular Blockchain, etc.

## Node API

After having instantiated a Node object, the following methods may be applied in order to achieve the desired functionality.

### Reading/Writing Methods

```python
write_data(data, chain, try_limit = 10, meta_data = "")
```

*Parameters:*

* ```data```: The data to be written into the blockchain, as a string.
* ```chain``: The blockchain to be altered.
* ```try_limit```: The maximum number of retries to be executed (default is 10).
* ```meta_data```: Metadata to be included in the Block object.

This method provides a higher-level wrapper over the ```mine_block_concurrently```
method from the Blockchain class. It can be used to write data into a given blockchain.
Writing an empty data string is not allowed.
Returns true if a block has been added successfully, else returns false.

-----------------------------------------------------------------------------------------------
```python
read_data_by_range(range, chain)
```

*Parameters:*

* ```range```: The number of content strings to be retreived from the ledger.
* ```chain```: A Blockchain object from which data should be read.

This method returns a number of content strings from the ledger, as a list in ascending order.
If the range exceeds the size of the ledger, an out-of-bounds exception is thrown.

---------------------------------------------------------------------------------------------------
```python
read_data_by_meta(meta, chain)
```
*Parameters:*

* ```meta```: A metadata string
* ```chain```: A Blockchain object

Get the data from all Block objects which share a common metadata attribute, in the order they appear in the ledger.

## Ledger Access Methods

These methods provide similar functionality to the ledger access methods from the ```Blockchain``` class, but they are not tied to any particular Blockchain and supply some different features.

---------------------------------------------------------------------------------------------------
```python
get_blocks_by_meta(meta, chain)
```
/*! Parameters:

* ```meta```: A metadata string
* ```chain```: A Blockchain object

Same as ```get_data_by_meta```, but returns Block objects instead of data strings.

----------------------------------------------------------------------------------------------------
```python
get_block_by_index(index, chain)
```
/*! Parameters:

* ```index```: The index of the relevant block in the Blockchain
* ```chain```: A Blockchain object

Get the Block object at the specified index in the ledger.


## Getters and Setters

### Setters

```python
set_node_addr(node_addr)
```

*Parameters:*

* ```node_addr```: Set the address of this Node object.

Set the address of this Node object.

----------------------------------------------------------------------------------------------------
```python
set_node_name(node_name)
```

Set the identifier of this node object.

### Getters

```python
get_node_addr()
```
Get the address (i.e. the SHA256 hash) of the Node object

----------------------------------------------------------------------------------------------------
```python
get_node_name()
```
Get the identifier stored in the Node object
