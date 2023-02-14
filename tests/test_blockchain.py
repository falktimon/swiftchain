import unittest
from swiftchain import Blockchain, Block, Node

class TestBlockchain(unittest.TestCase):

    def test_verify_block(self):

        blockchain = Blockchain()

        # A block that was not mined on the chain cannot be verified:
        wrong_block = Block(data="Wrong block", user_addr="Bad person")

        correct_block = blockchain.mine_block(data="Correct block", node_addr="Good person")

        self.assertFalse(blockchain.verify_block(wrong_block))
        self.assertTrue(blockchain.verify_block(correct_block))

    def test_get_ledger_size(self):

        tester_node = Node("Tester")
        blockchain = Blockchain()

        self.assertEqual(1, blockchain.get_ledger_size())

        for i in range(100): 
            tester_node.write_data(data=str(i), chain=blockchain)

        self.assertEqual(101, blockchain.get_ledger_size())

    def test_get_ledger(self):

        blockchain = Blockchain()
        g_block_hash = blockchain.get_last_block().get_block_hash()

        test_block = None

        while not test_block:
            test_block = blockchain.mine_block(data="Test", node_addr="8367")
        
        ledger = blockchain.get_ledger()

        self.assertIsNotNone(ledger[g_block_hash])

    def test_get_try_limit(self):

        blockchain = Blockchain(try_limit=10000)
        self.assertEqual(10000, blockchain.get_try_limit())

    def test_get_difficulty(self):

        tester_node = Node("Tester")
        blockchain = Blockchain(diff_threshold=10)

        self.assertEqual(1, blockchain.get_difficulty())

        for i in range(105):
            tester_node.write_data(data=str(i), chain=blockchain)

        self.assertTrue(blockchain.get_difficulty() > 9)

    def test_get_block(self):

        blockchain = Blockchain()
        g_block = blockchain.get_last_block()

        self.assertEqual(g_block, blockchain.get_block(g_block.get_block_hash()))

    def test_get_redux_time(self):

        blockchain = Blockchain(redux_time=0.5)
        self.assertEqual(1800000, blockchain.get_redux_time())

    def test_get_last_block(self):

        blockchain = Blockchain()
        g_block = blockchain.get_last_block()
        test_block = None

        while not test_block:
            test_block = blockchain.mine_block(data="Test", node_addr="8367")

        self.assertEqual(test_block, blockchain.get_last_block())
        self.assertNotEqual(g_block, blockchain.get_last_block())

    def test_find_consensus(self):

        tester_node = Node("Tester")

        blockchain1 = Blockchain(diff_threshold=10, g_data="Fiat Lux!")
        blockchain2 = Blockchain(diff_threshold=7, g_data="Fiat Lux!")

        for i in range(100):
            tester_node.write_data(data=str(i), chain=blockchain1)
        
        for i in range(80):
            tester_node.write_data(data=str(i), chain=blockchain2)

        # Blockchain 2 has a higher difficulty threshold, thus it will have a 
        # higher cumulative proof-of-work despite mining fewer blocks:
        self.assertFalse(blockchain2.find_consensus(blockchain1))
        self.assertTrue(blockchain1.find_consensus(blockchain2))
        self.assertEqual(blockchain1.get_last_block(), blockchain2.get_last_block())
    
    def test_mine_block(self):

        blockchain = Blockchain()
        test_block = None

        while not test_block:
            test_block = blockchain.mine_block(data="Test", node_addr="8367")

        self.assertIsNotNone(test_block)
        self.assertTrue(blockchain.verify_block(test_block))
        self.assertEqual(blockchain.get_last_block(), test_block)

    def test_mine_block_concurrently(self):

        blockchain = Blockchain()
        test_block = None

        while not test_block:
            test_block = blockchain.mine_block_concurrently(data="Test", node_addr="8367")

        self.assertIsNotNone(test_block)
        self.assertTrue(blockchain.verify_block(test_block))
        self.assertEqual(blockchain.get_last_block(), test_block)

if __name__ == '__main__': unittest.main()