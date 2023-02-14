import unittest
from swiftchain import Node, Blockchain

class TestNode(unittest.TestCase):

    def test_get_node_name(self):

        tester_node = Node("Tester")
        self.assertEqual("Tester", tester_node.get_node_name())

    def test_get_data_by_range(self):

        tester_node = Node("Tester")
        blockchain = Blockchain(g_data="Test", node_addr=tester_node.get_node_addr())

        for i in range(100): tester_node.write_data(data=str(i), chain=blockchain)
        output = tester_node.read_data_by_range(5, chain=blockchain)

        self.assertEqual(['95', '96', '97', '98', '99'], output)

    def test_write_data(self):

        tester_node = Node("Tester")
        blockchain = Blockchain(g_data="Test", node_addr=tester_node.get_node_addr())

        tester_node.write_data(data="Written successfully", chain=blockchain)
        last_block = blockchain.get_last_block()

        self.assertEqual("Written successfully", last_block.get_data())

