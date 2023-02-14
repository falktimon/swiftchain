from swiftchain import Blockchain, Node

tester = Node("Tester")
chain = Blockchain()

for i in range(100): tester.write_data(data=str(i), chain=chain)

ids = [block.get_data() for block in chain.get_blocks_by_range(10)]

print(ids)

