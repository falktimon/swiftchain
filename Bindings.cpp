#ifndef PYBIND_HEADER
#define PYBIND_HEADER
#include "pybind11/include/pybind11/pybind11.h"
#include "pybind11/include/pybind11/chrono.h"
#include "pybind11/include/pybind11/stl.h"
#endif

#ifndef BLOCKCHAIN_CPP
#define BLOCKCHAIN_CPP
#include "Blockchain.cpp"
#endif

#ifndef BLOCK_CPP
#define BLOCK_CPP
#include "Block.cpp"
#endif

#ifndef NODE_CPP
#define NODE_CPP
#include "Node.cpp"
#endif


namespace py = pybind11;

PYBIND11_MODULE(swiftchain, m)
{
     py::class_<Block>(m, "Block")
          .def(py::init<Block *, std::string, 
               std::string, long unsigned int, long unsigned int>(),
               "Common Block constructor. Creates a block if given a previous block.",
               py::arg("prev"), py::arg("data"), py::arg("userAddr"),
               py::arg("nonce"), py::arg("difficulty"))
          .def(py::init<std::string, std::string>(), 
               "Genesis constructor. Generate a Genesis block. This is called in the Blockchain constructor.",
               py::arg("data"), py::arg("userAddr"))
          .def("getBlockID", &Block::getBlockID,
               "Get the unique ID of the relevant block.")
          .def("getData", &Block::getData, "Get the content stored in this Block.")
          .def("getBlockHash", &Block::getBlockHash,
               "Return the hash of this Block.")
          .def("getPrevHash", &Block::getPrevHash,
               "Return the previous hash that this Block stores.")
          .def("getDifficulty", &Block::getDifficulty,
               "Return the difficulty under which this Block was mined.")
          .def("getNonce", &Block::getNonce, 
               "Return the nonce with which this Block was mined.")
          .def("setBlockID", &Block::setBlockID, 
               "Set the unique ID of this block. \
               This function is mostly for internal use, as it may break the ledger.",
               py::arg("blockID"));

     py::class_<Node>(m, "Node")
          .def(py::init<std::string>(), py::arg("nodeName"))
          .def("getNodeName", &Node::getNodeAddress, 
               "Return the address of the relevant node.")
          .def("getNodeAddress", &Node::getNodeName, 
               "Return the name of the relevant node.")
          .def("setNodeName", &Node::setNodeName, 
               "Set the name of the current node.", py::arg("nodeName"))
          .def("setNodeAddress", &Node::setNodeAddress, 
               "Manually set the node address.",py::arg("nodeAddress"))
          .def("readDataByRange", &Node::readDataByRange, 
               "Return the data stored in a range of blocks.", 
               py::arg("range"), py::arg("chain"))
          .def("writeData", &Node::writeData, 
               "Write data into the blockchain from a node.",
               py::arg("data"), py::arg("chain"), py::arg("maxTries") = 10);
     
     py::class_<Blockchain>(m, "Blockchain")
          .def(py::init<long unsigned int, unsigned int, std::string, std::string, float>(),
               "Constructor for blockchain objects. User address and Genesis data must be set.",
               py::arg("tryLimit") = 10000, py::arg("diffThreshold") = 100, py::arg("userAddr") = "UNSET", 
               py::arg("gData") = "", py::arg("reduxTime") = 0.5)
          .def("getDifficulty", &Blockchain::getDifficulty, 
               "Return the current difficulty to be satisfied in order to mine a block.")
          .def("getLedger", &Blockchain::getLedger,
               "Return the current ledger as a Ledger object.")
          .def("verifyBlock", &Blockchain::verifyBlock, 
               "Verify a current block against the entire blockchain.", py::arg("block"))
          .def("mineBlock", &Blockchain::mineBlock, 
               "Mine a block on a single CPU core. Returns None on failure, Block on success.",
               py::arg("data"), py::arg("nodeAddress"))
          .def("mineBlockConcurrently", &Blockchain::mineBlockConcurrently, 
               "Mine a block in a parallelized manner. Returns None on failure, Block on success.",
               py::arg("data"), py::arg("nodeAddress"))
          .def("findConsensus", &Blockchain::findConsensus, 
               "Find the Nakamoto consensus between two ledgers and replaces ledger \
               if foreign ledger possesses higher cumulative proof-of-work. \
               Returns ledger on success, None on failure.", py::arg("fChain"))
          .def("getBlock", &Blockchain::getBlock, "Get a block by hash.", py::arg("hash"))
          .def("getLastBlock", &Blockchain::getLastBlock, "Get the last block in the ledger.")
          .def("setDifficultyLimit", &Blockchain::setDifficultyLimit, 
               "Set the difficulty threshold. \
               (The number of blocks at which the difficulty is raised.)", py::arg("dLimit"))
          .def("setTryLimit", &Blockchain::setTryLimit, 
               "Set the max. number of tries per mining attempt.", py::arg("tryLimit"))
          .def("setDifficulty", &Blockchain::setDifficulty, 
               "Set the difficulty of the blockchain.", py::arg("diff"))
          .def("getLedgerSize", &Blockchain::getLedgerSize,
               "Get the size of the current ledger.");
}

