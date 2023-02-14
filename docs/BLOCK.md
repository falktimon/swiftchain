---
# Index
- [Block](#block)
  * [Block Constructor](#block-constructor)
- [Block API](#block-api)
  * [Various Methods](#various-methods)
  * [Getters and Setters](#getters-and-setters)
    + [Getters](#getters)
    + [Setters](#setters)
---

# Block

Objects of the Block class form the basis of any Blockchain. Blocks contain a variety of different data fields, which can be accessed using the methods this API provides.
The Block class usually doesn't have to be included in swiftchain apps, unless a custom Blockchain is to be implemented using this primitive.

This class exposes the following methods:

## Block Constructor

The Block class possesses two constructors. One is used to generate a Genesis Block, the other one to create a Block object by providing a previous Block.

**Method Signature (Genesis):**

```python
Block(data, node_addr)
```

**Parameters:**

* ```data```: Some content to be contained within the Block.
* ```node_addr```: The address of the Node creating this block.

This is the Genesis constructor. It can be used to construct the first Block in a Blockchain. This constructor is called in the ```Blockchain``` constructor.

**Method Signature (Common):**

```python
Block(prev, data, node_addr, nonce, difficulty, blockchain_id = "0", meta_data = "")
```

**Parameters:**

* ```prev```: An instance of a Block object. 
* ```data```: Some content to be contained within the Block.
* ```node_addr```: The node_address of the Node creating this block.
* ```nonce```: A number used to generate varying hashes.
* ```difficulty```: The current difficulty level.
* ```blockchain_id```: The ID of the Blockchain on which this Block was mined.
* ```meta_data```: Metadata to be stored in this Block. 

This is the common constructor used for mining blocks. Note that there are no defaults for any parameter in this constructor.

# Block API

The Block class exposes the following methods:

## Various Methods

```python
generate_block_hash(block)
```

*Parameters:* 
* ```block```: Some Block object to be hashed.

Generate a SHA256 hash string from a given Block object.

-------------------------------------------------------------
```python
generate_timestamp()
```
Returns the current UNIX time as a string.

-------------------------------------------------------------

## Getters and Setters

### Getters

```python
get_block_id(void)
```
Returns the unique ID of the Block object as an integer.

---------------------------------------------------------------
```python
get_data()
```
Get the content contained within the Block object. 

----------------------------------------------------------------
```python
get_block_hash()
```
Get the hash of the Block object.

-----------------------------------------------------------------
```python
get_prev_hash()
```
Get the previous hash stored within in Block object.

------------------------------------------------------------------
```python
get_difficulty()
```
Get the difficulty level under which the current block was mined.

-------------------------------------------------------------------
```python
get_nonce()
```
Return the nonce with which the Block object was mined.

-------------------------------------------------------------------
```python
get_node_address()
```
Get the address of the node that mined this block.

-------------------------------------------------------------------
```python
get_timestamp()
```
Get the UNIX timestamp contained within this string.

-------------------------------------------------------------------
```python
get_blockchain_id()
```
Get the ID of the blockchain associated with this Block object.

-------------------------------------------------------------------
```python
get_meta_data()
```
Get the meta data stored within the Block object.

------------------------------------------------------------------
```python
get_meta_data()
```
Get the meta data stored within the Block object.

### Setters

Be careful when using these setters, as most attributes of Block objects are inferred automatically. Setting a wrong value here may break the Blockchain.

```python
set_block_id(long unsigned int)
```
*Parameters:* A unique integer to be used as a block identifier.

Set the block ID of the Block object.

-------------------------------------------------------------------
```python
set_blockchain_id(string)
```
*Parameters:* A blockchain ID as a string.
Specify a blockchain which this Block object is contained with.

--------------------------------------------------------------------
```python
set_node_addr(string)
```
*Parameters:* A Node address as a string.
Set the Node address associated with this Block object.

--------------------------------------------------------------------
```python
set_data(string)
```
Set the content of this Block object.

---------------------------------------------------------------------
```python 
set_meta_data(string)
```
Set the metadata stored within the Block object.