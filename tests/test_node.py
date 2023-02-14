# Copyright 2020 Falk Spickenbaum
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import unittest
from swiftchain import Node, Blockchain

class TestNode(unittest.TestCase):

    def test_get_node_name(self):

        tester_node = Node("Tester")
        self.assertEqual("Tester", tester_node.get_node_name())

    def test_read_data_by_range(self):

        tester_node = Node("Tester")
        blockchain = Blockchain(g_data="Test", node_addr=tester_node.get_node_addr())

        for i in range(100): tester_node.write_data(data=str(i), chain=blockchain)
        output = tester_node.read_data_by_range(5, chain=blockchain)

        with self.assertRaises(Exception) as context:
            tester_node.read_data_by_range(range=102, chain=blockchain)

        self.assertTrue("exceeds size of ledger." in str(context.exception))
        self.assertEqual(['95', '96', '97', '98', '99'], output)

    def test_write_data(self):

        tester_node = Node("Tester")
        blockchain = Blockchain(g_data="Test", node_addr=tester_node.get_node_addr())

        tester_node.write_data(data="Written successfully", chain=blockchain)
        last_block = blockchain.get_last_block()

        self.assertEqual("Written successfully", last_block.get_data())

    def test_get_block_by_index(self):

        tester_node = Node("Tester")
        blockchain = Blockchain()

        for i in range(10):
            tester_node.write_data(data=str(i), chain=blockchain, meta_data="Meta " + str(i))

        block = tester_node.get_block_by_index(index=7, chain=blockchain)

        self.assertEqual('6', block.get_data())

        with self.assertRaises(Exception) as context:
            tester_node.get_block_by_index(index=22, chain=blockchain)

        self.assertTrue("Requested index exceeds size of ledger." in str(context.exception))

    def test_read_data_by_meta(self):

        tester_node = Node("Tester")
        blockchain = Blockchain()

        for i in range(10):
            tester_node.write_data(data=str(i), chain=blockchain, meta_data="Meta " + str(i))

        data = tester_node.read_data_by_meta(meta="Meta 9", chain=blockchain)
        self.assertEqual(["9"], data)

        with self.assertRaises(Exception) as context:
            tester_node.read_data_by_meta(meta="Not contained inside", chain=blockchain)

        self.assertTrue("Ledger does not contain" in str(context.exception))

    def test_get_blocks_by_meta(self):

        tester_node = Node("Tester")
        blockchain = Blockchain()

        for i in range(10):
            tester_node.write_data(data=str(i), chain=blockchain, meta_data="Meta " + str(i))

        blocks = tester_node.get_blocks_by_meta(meta="Meta 2", chain=blockchain)

        with self.assertRaises(Exception) as context:
            tester_node.get_blocks_by_meta(meta="Not contained inside", chain=blockchain)

        self.assertTrue("Ledger does not contain" in str(context.exception))
        self.assertEqual("2", blocks[0].get_data())


if __name__ == '__main__': unittest.main()
