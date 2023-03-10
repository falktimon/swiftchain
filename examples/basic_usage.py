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
