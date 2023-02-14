from swiftchain import Block
import unittest

class TestBlock(unittest.TestCase):

    def test_set_data(self):

        block = Block(data="Old data", user_addr="Tester")
        block.set_data("Testing")

        self.assertEqual("Testing", block.get_data())
    
    def test_set_node_addr(self):

        block = Block(data="Some Data", user_addr="Tester")
        block.set_node_addr(node_addr="New address")

        self.assertEqual("New address", block.get_node_addr())
    
    def test_set_block_id(self):

        block = Block(data="Some Data", user_addr="Tester")
        block.set_block_id(block_id=2)

        self.assertEqual(2, block.get_block_id())

    def test_get_block_hash(self):

        block = Block(data="Some Data", user_addr="Tester")
        self.assertIsNotNone(block.get_block_hash())

    def test_get_nonce(self):

        block = Block(data="Some Data", user_addr="Tester")
        self.assertEqual(0, block.get_nonce())

    def test_get_difficulty(self):

        block = Block(data="Some Data", user_addr="Tester")
        self.assertEqual(0, block.get_difficulty())

    def test_get_data(self):

        block = Block(data="Some Data", user_addr="Tester")
        self.assertEqual("Some Data", block.get_data())

if __name__ == '__main__': unittest.main()
