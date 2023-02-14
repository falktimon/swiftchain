import unittest
from swiftchain import Blockchain

class TestBlockchain(unittest.TestCase):

    def test_mine_block_concurrently(self):

        blockchain = Blockchain()
        test_block = None

        while not test_block:
            test_block = blockchain.mine_block_concurrently(data="Test", node_addr="8367")

        self.assertIsNotNone(test_block)
        # Fails because blocks are not in order:
        self.assertEqual(blockchain.get_last_block().get_block_hash(), test_block.get_block_hash())

if __name__ == '__main__': unittest.main()