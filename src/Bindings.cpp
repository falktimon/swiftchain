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
          .def("get_block_id", &Block::get_block_id,
               "Get the unique ID of the relevant block.")
          .def("get_data", &Block::get_data, "Get the content stored in this Block.")
          .def("get_block_hash", &Block::get_block_hash,
               "Return the hash of this Block.")
          .def("get_prev_hash", &Block::get_prev_hash,
               "Return the previous hash that this Block stores.")
          .def("get_difficulty", &Block::get_difficulty,
               "Return the difficulty under which this Block was mined.")
          .def("get_nonce", &Block::get_nonce, 
               "Return the nonce with which this Block was mined.")
          .def("set_block_id", &Block::set_block_id, 
               "Set the unique ID of this block. \
               This function is mostly for internal use, as it may break the ledger.",
               py::arg("blockID"));

     py::class_<Node>(m, "Node")
          .def(py::init<std::string>(), py::arg("nodeName"))
          .def("get_node_name", &Node::get_node_address, 
               "Return the address of the relevant node.")
          .def("get_node_address", &Node::get_node_name, 
               "Return the name of the relevant node.")
          .def("set_node_name", &Node::set_node_name, 
               "Set the name of the current node.", py::arg("nodeName"))
          .def("set_node_address", &Node::set_node_address, 
               "Manually set the node address.",py::arg("nodeAddress"))
          .def("read_data_by_range", &Node::read_data_by_range, 
               "Return the data stored in a range of blocks.", 
               py::arg("range"), py::arg("chain"))
          .def("write_data", &Node::write_data, 
               "Write data into the blockchain from a node.",
               py::arg("data"), py::arg("chain"), py::arg("maxTries") = 10);
     
     py::class_<Blockchain>(m, "Blockchain")
          .def(py::init<long unsigned int, unsigned int, std::string, std::string, float>(),
               "Constructor for blockchain objects. User address and Genesis data must be set.",
               py::arg("tryLimit") = 10000, py::arg("diffThreshold") = 100, py::arg("userAddr") = "UNSET", 
               py::arg("gData") = "", py::arg("reduxTime") = 0.5)
          .def("get_difficulty", &Blockchain::get_difficulty, 
               "Return the current difficulty to be satisfied in order to mine a block.")
          .def("get_ledger", &Blockchain::get_ledger,
               "Return the current ledger as a Ledger object.")
          .def("verify_block", &Blockchain::verify_block, 
               "Verify a current block against the entire blockchain.", py::arg("block"))
          .def("mine_block", &Blockchain::mine_block, 
               "Mine a block on a single CPU core. Returns None on failure, Block on success.",
               py::arg("data"), py::arg("nodeAddress"))
          .def("mine_block_concurrently", &Blockchain::mine_block_concurrently, 
               "Mine a block in a parallelized manner. Returns None on failure, Block on success.",
               py::arg("data"), py::arg("nodeAddress"))
          .def("find_consensus", &Blockchain::find_consensus, 
               "Find the Nakamoto consensus between two ledgers and replaces ledger \
               if foreign ledger possesses higher cumulative proof-of-work. \
               Returns ledger on success, None on failure.", py::arg("fChain"))
          .def("get_block", &Blockchain::get_block, "Get a block by hash.", py::arg("hash"))
          .def("get_last_block", &Blockchain::get_last_block, "Get the last block in the ledger.")
          .def("set_difficulty_limit", &Blockchain::set_difficulty_limit, 
               "Set the difficulty threshold. \
               (The number of blocks at which the difficulty is raised.)", py::arg("dLimit"))
          .def("set_try_limit", &Blockchain::set_try_limit, 
               "Set the max. number of tries per mining attempt.", py::arg("tryLimit"))
          .def("set_difficulty", &Blockchain::set_difficulty, 
               "Set the difficulty of the blockchain.", py::arg("diff"))
          .def("get_ledger_size", &Blockchain::get_ledger_size,
               "Get the size of the current ledger.");
}

