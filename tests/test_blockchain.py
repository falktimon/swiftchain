import unittest
from swiftchain import Blockchain, Block

class TestBlockchain(unittest.TestCase):

    def test_verify_block(self):

        blockchain = Blockchain()

        # A block that was not mined on the chain cannot be verified:
        wrong_block = Block(data="Wrong block", user_addr="Bad person")

        correct_block = blockchain.mine_block(data="Correct block", node_addr="Good person")

        self.assertFalse(blockchain.verify_block(wrong_block))
        self.assertTrue(blockchain.verify_block(correct_block))


    def test_mine_block_concurrently(self):

        blockchain = Blockchain()
        test_block = None

        while not test_block:
            test_block = blockchain.mine_block_concurrently(data="Test", node_addr="8367")

        self.assertIsNotNone(test_block)
        self.assertEqual(blockchain.get_last_block(), test_block)

if __name__ == '__main__': unittest.main()