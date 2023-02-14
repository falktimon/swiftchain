from swiftchain import Blockchain, Node

# Create a node, which will mine blocks on the given chains:
user = Node("Tester")

# Create a blockchain with a difficulty threshold of 10,
# meaning that the difficulty is raised every 10 blocks.
first_chain = Blockchain(diff_threshold=10, g_data="First Blockchain", 
                        node_addr=user.get_node_addr())

# Create a blockchain with a difficulty threshold of 7,
# meaning that the difficulty is raised every 7 blocks.
second_chain = Blockchain(diff_threshold=7, g_data="Second Blockchain", 
                        node_addr=user.get_node_addr())

# Mine 100 blocks on the first chain and 80 blocks on the second chain:
for i in range(100):
    if i < 80: user.write_data(data=str(i), chain=second_chain)
    user.write_data(data=str(i), chain=first_chain)

print(f"Last Block ID of first chain (before consensus): {first_chain.get_last_block().get_block_id()}")
print(f"Last Block ID of second chain (before consensus): {second_chain.get_last_block().get_block_id()}\n")

print("Finding consensus...\n")

# Even though there are more blocks in the first chain,
# the ledger of this chain will be replaced by the second ledger.
# This is because the second chain has a higher cumulative proof-of-work,
# since the difficulty is raised more often:
if not second_chain.find_consensus(f_chain=first_chain):
    first_chain.find_consensus(f_chain=second_chain)

""" We've found the consensus and brought both chains into the same state! """

print(f"Last Block ID of first chain (after consensus): {first_chain.get_last_block().get_block_id()}")
print(f"Last Block ID of second chain (after consensus): {second_chain.get_last_block().get_block_id()}")
