#ifndef PYBIND_HEADER
#define PYBIND_HEADER
#include "pybind11/include/pybind11/pybind11.h"
#include "pybind11/include/pybind11/chrono.h"
#include "pybind11/include/pybind11/stl.h"
#include "pybind11/include/pybind11/stl_bind.h"
#endif

#ifndef BLOCKCHAIN_CPP
#define BLOCKCHAIN_CPP
#include "blockchain.cpp"
#endif

#ifndef BLOCK_CPP
#define BLOCK_CPP
#include "block.cpp"
#endif

#ifndef NODE_CPP
#define NODE_CPP
#include "node.cpp"
#endif

#ifndef HASH_HEADER
#define HASH_HEADER
#include "hashing_util.hpp"
#endif

#ifndef POW_HEADER
#define POW_HEADER
#include "pow_util.hpp"
#endif

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::map<std::string, Block *>);

PYBIND11_MODULE(swiftchain, m)
{
     py::bind_map<std::map<std::string, Block *>>(m, "Ledger");

     py::class_<Block>(m, "Block")
          .def(py::init<Block *, std::string, 
               std::string, long unsigned int, long unsigned int>(),
               "Common Block constructor. Creates a block if given a previous block.",
               py::arg("prev"), py::arg("data"), py::arg("user_addr"),
               py::arg("nonce"), py::arg("difficulty"))
          .def(py::init<std::string, std::string>(), 
               "Genesis constructor. Generate a Genesis block. This is called in the Blockchain constructor.",
               py::arg("data"), py::arg("user_addr"))
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
               py::arg("block_id"))
          .def("get_blockchain_id", &Block::get_blockchain_id,
               "Get the ID of the blockchain on which this block has mined.");

     py::class_<Node>(m, "Node")
          .def(py::init<std::string>(), py::arg("node_name"))
          .def("get_node_name", &Node::get_node_address, 
               "Return the address of the relevant node.")
          .def("get_node_address", &Node::get_node_name, 
               "Return the name of the relevant node.")
          .def("set_node_name", &Node::set_node_name, 
               "Set the name of the current node.", py::arg("node_name"))
          .def("set_node_address", &Node::set_node_address, 
               "Manually set the node address.",py::arg("node_addr"))
          .def("read_data_by_range", &Node::read_data_by_range, 
               "Return the data stored in a range of blocks.", 
               py::arg("range"), py::arg("chain"))
          .def("write_data", &Node::write_data, 
               "Write data into the blockchain from a node.",
               py::arg("data"), py::arg("chain"), py::arg("max_tries") = 10);
     
     py::class_<Blockchain>(m, "Blockchain")
          .def(py::init<long unsigned int, unsigned int, std::string, std::string, float>(),
               "Constructor for blockchain objects. User address and Genesis data must be set.",
               py::arg("try_limit") = 10000, py::arg("diff_threshold") = 100, py::arg("node_addr") = "UNSET", 
               py::arg("g_data") = "", py::arg("redux_time") = 0.5)
          .def("get_difficulty", &Blockchain::get_difficulty, 
               "Return the current difficulty to be satisfied in order to mine a block.")
          .def("get_ledger", &Blockchain::get_ledger,
               "Return the current ledger as a Ledger object.")
          .def("verify_block", &Blockchain::verify_block, 
               "Verify a current block against the entire blockchain.", py::arg("block"))
          .def("mine_block", &Blockchain::mine_block, 
               "Mine a block on a single CPU core. Returns None on failure, Block on success.",
               py::arg("data"), py::arg("node_addr"))
          .def("mine_block_concurrently", &Blockchain::mine_block_concurrently, 
               "Mine a block in a parallelized manner. Returns None on failure, Block on success.",
               py::arg("data"), py::arg("node_addr"))
          .def("find_consensus", &Blockchain::find_consensus, 
               "Find the Nakamoto consensus between two ledgers and replaces ledger \
               if foreign ledger possesses higher cumulative proof-of-work. \
               Returns ledger on success, None on failure.", py::arg("f_chain"))
          .def("get_block", &Blockchain::get_block, "Get a block by hash.", py::arg("hash"))
          .def("get_last_block", &Blockchain::get_last_block, "Get the last block in the ledger.", 
               py::return_value_policy::reference)
          .def("set_difficulty_limit", &Blockchain::set_difficulty_limit, 
               "Set the difficulty threshold. \
               (The number of blocks at which the difficulty is raised.)", py::arg("diff_threshold"))
          .def("set_try_limit", &Blockchain::set_try_limit, 
               "Set the max. number of tries per mining attempt.", py::arg("try_limit"))
          .def("set_difficulty", &Blockchain::set_difficulty, 
               "Set the difficulty of the blockchain.", py::arg("diff"))
          .def("get_ledger_size", &Blockchain::get_ledger_size,
               "Get the size of the current ledger.")
          .def("get_try_limit", &Blockchain::get_try_limit, "Get the maximum number of tries per mining attempt.")
          .def("set_redux_time", &Blockchain::set_redux_time, 
               "Set the time after which the difficulty is decreased (in hours).",
               py::arg("hours"))
          .def("get_redux_time", &Blockchain::get_redux_time, 
               "Get the time after which the difficulty is decreased (in milliseconds).")
          .def("get_blockchain_id", &Blockchain::get_blockchain_id, 
               "Get the identifying hash of this blockchain.");
     
          m.def("find_trailing_zeros", &find_trailing_zeros,
               "Find the number of trailing zeros in a hash.",
               py::arg("hash"))
          .def("convert_to_bin", &convert_to_bin,
               "Convert a hexadecimal string to a binary string.",
               py::arg("hash"))
          .def("run_hash_cash", &run_hash_cash, 
               "Run hashcash on a block. Returns true if block hash \
               matches the difficulty stored within the block. Else returns false.",
               py::arg("block"))
          .def("verify_attempt", &verify_attempt,
               "Try to verify a block. Entry point for POW algorithm.",
               py::arg("block"))
          .def("generate_sha_hash", &generate_sha_hash,
               "Generate a SHA256 hash from a given string.",
               py::arg("data"));
}
