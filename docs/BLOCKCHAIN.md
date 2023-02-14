---
# Index

- [The Blockchain Class](#the-blockchain-class)
  - [Blockchain Constructor](#blockchain-constructor)
- [Blockchain API](#blockchain-api)
  - [Mining Methods](#mining-methods)
  - [Ledger Access Methods](#ledger-access-methods)
  - [Various Methods](#various-methods)
  - [Getters and Setters](#getters-and-setters)
---
# The Blockchain Class

The Blockchain class is used to generate a Blockchain object, which contains a ledger and various methods for interacting with said ledger.
A ledger is basically a collection of Block objects which form the Blockchain itself. This class exposes the following methods:

## Blockchain Constructor

The Blockchain constructor is used to instantiate a new Blockchain object.
Generally speaking, all arguments in the constructor are optional and have default values. However, it usually makes sense to configure the Blockchain for a specific task instead of using the defaults. Especially ```node_addr``` and ```g_data``` should always be set.

**Method Signature:**

```python
Blockchain(try_limit = 10000, diff_threshold = 100, node_addr = "UNSET", g_data = "", redux_time = 0.5)
```
**Parameters:** 

* ```try_limit```: The maximum number of hashes generated for each mining attempt.
* ```diff_threshold```: The number of Block objects in the ledger at which the difficulty is incremented.
* ```node_addr```: Some user identifier, as a string. 
* ```genesis_data```: The data to be stored in the Genesis block.
* ```diff_redux_time```: The timespan in hours that has to pass between Blocks in order for the difficulty to be halfed.

The ```try_limit``` and ``diff_threshold``` parameters are the parameters that specify how hard it is to mine a new block on the Blockchain. These parameters cannot be zero.

# Blockchain API

After having instantiated a Blockchain object, the following methods may be applied in order to achieve the desired functionality.

All parameters in this API can be called with keyword arguments.

## Mining Methods

For most applications which require mining, it is preferable to use the higher-level ```write_data``` method from the ```Node``` class.
If more precise control over the mining process is required, use these methods.

-------------------------------------------------------------------------------------------
```python
mine_block(data, node_addr, meta_data = "")
```
*Parameters:* 

* ```data```: Some data to be stored in the new block.
* ```node_address```: Some user identifier as a string.
* ```meta_data```: The metadata to be contained in the new block.

This method tries to mine a Block object on this Blockchain. This method does not take
advantage of multi-processing. If a block should be mined in parallel, 
use the mine_block_concurrently(string, string) method.
Appends a new Block to the ledger and returns it on success, returns ```None``` on failure.

-----------------------------------------------------------------------------------------------------
```python
mine_block_concurrently(data, node_addr, meta_data = "")
```
*Parameters:*

* ```data```: The data to be stored in the new block, as a string.
* ```node_address```: A string identifier of a blockchain user. 
* ```meta_data```: The metadata to be contained in the new block.

This method tries to mine a single block in multiple threads. 
If an attempt should be started on a single thread, use ```mine_block```.

## Ledger Access Methods

-------------------------------------------------------------------------------------------------
```python
get_last_block()
````

Get the last block in the ledger, meaning the block with the highest block ID.

-------------------------------------------------------------------------------------------------
```python
get_blocks_by_range(range)
```

*Parameters:*

* ```range```: The number of blocks to be retrieved from the ledger, starting from the last block.

Returns a range of Blocks from the ledger as a vector. If the range exceeds the size of
the ledger, an out_of_bounds exception is thrown. Blocks are ordered by ID in ascending order.

----------------------------------------------------------------------------------------------------
```python
get_block(hash)
```

*Parameters:* 

* ```hash```: A block hash as a string.

Find a Block object in the ledger by hash.
Returns a Block object on success, ```None``` on failure.

-----------------------------------------------------------------------------------------------------
```python
get_block_by_index(index)
```

*Parameters:* 

* ```index```: The index of the relevant Block object in the Blockchain

Get the Block object at the specified index in the ledger.

## Various Methods

------------------------------------------------------------------------------------------------------
```python
find_consensus(f_chain)
```

*Parameters:* 

* ```chain```: Another Blockchain object to be used in the consensus algorithm.

This method attempts to find a consensus between to ledgers by comparing the cumulative 
proof-of-work contained in the ledger.
If the foreign Blockchain contains more cumulative proof-of-work, the ledger of the calling Blockchain object 
is replaced. If this is the case, this method returns true. Else it returns false.*/

---------------------------------------------------------------------------------------------------
```python
adjust_difficulty()
```
Checks if the conditions for adjusting the difficulty of the Blockchain are met and acts accordingly.

----------------------------------------------------------------------------------------------------
```python
verify_block(block)
```
*Parameters:* 

* ```block```: Any block object to be validated.
 
Tries to verify a given block against the whole Blockchain.
Returns true on success, false on failure.

------------------------------------------------------------------------------------------------------

## Getters and Setters
Getters and Setters for the Blockchain class.

### Setters
------------------------------------------------------------------------------------------------------
```python
set_try_limit(try_limit)
```
*Parameters:*

* ```try_limit```: The new try limit as an integer.

Set the maximum number of hashes to be generated per mining attempt. After this number is exceeded mining will stop, regardless if a new block has been mined or not.

------------------------------------------------------------------------------------------------------
```python
set_diff_threshold(diff_threshold)
```
*Parameters:*

* ```diff_threshold```: The new difficulty threshold as an integer.

Set the number of blocks contained with the ledger at which the difficulty is raised:

------------------------------------------------------------------------------------------------------
```python
set_difficulty(difficulty)
```
*Parameters:*

* ```difficulty```: The new difficulty as an integer.

Set the current difficulty of the blockchain

-------------------------------------------------------------------------------------------------------
```python
set_redux_time(redux_time)
```

*Parameters:*

* ```redux_time```: The new timespan as an integer, in hours.

Set the timespan in hours after which the difficulty is halfed.

### Getters

--------------------------------------------------------------------------------------------------------
```python
get_ledger()
```
Return the ledger contained in the Blockchain object.

--------------------------------------------------------------------------------------------------------
```python
get_difficulty()
```
Get the current difficulty contained within the Blockchain object.

--------------------------------------------------------------------------------------------------------
```python
get_ledger_size()
```
Get the current size of the ledger.

--------------------------------------------------------------------------------------------------------
```python
get_diff_threshold()
```
Get the current number of Blocks at which the difficulty is raised.

--------------------------------------------------------------------------------------------------------
```python
get_try_limit()
```
Get the maximum number of hashes generated per mining attempt.

--------------------------------------------------------------------------------------------------------
```python
get_redux_time()
```
Get the timespan after which the difficulty is halfed.

--------------------------------------------------------------------------------------------------------
```python
get_blockchain_id()
```
Get the identifier of the Blockchain object.
