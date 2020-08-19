//
// Created by artem on 13.08.20.
//
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstring>

#include "Blockchain.h"
#include "utils.h"

using namespace boost::property_tree;

Blockchain *Blockchain::blockchain = nullptr;

Blockchain::Blockchain() : isLastBlockHashValid(true) {
  memset(&lastBlockHash, 0, sizeof(Hash));
  updateDifficulty();
}

Blockchain *Blockchain::getInstance() {
  if(blockchain == nullptr) {
    blockchain = new Blockchain();
  }
  return blockchain;
}

void Blockchain::addBlock(const Block &block) {
  blocks.push_back(block);
  isLastBlockHashValid = false;
  updateDifficulty();
}

void Blockchain::updateDifficulty() {
  difficulty = 20 + (blocks.size() % 3);
}

Hash Blockchain::getLastBlockHash() {
  if(!isLastBlockHashValid) {
    lastBlockHash = sha256bytes(blocks.back().getRawData());
    isLastBlockHashValid = true;
  }
  return lastBlockHash;
}

uint32_t Blockchain::getLastBlockHeight() const {
  return blocks.size();
}

uint32_t Blockchain::getDifficulty() const {
  return difficulty;
}

std::string Blockchain::jsonify() const {
  ptree pt;

  for(const auto &b : blocks) {
    ptree blockTree;
    ptree transactionTree;
    ptree inputsTree;
    ptree outputsTree;

    for(const auto &input : b.transaction.inputs) {
      ptree inputTree;
      inputTree.put("hash", "0x" + hashToHexStr(input.hash));
      inputTree.put("sig", "0x" + sigToHexStr(input.sig));
      inputTree.put("index", input.index);
      inputsTree.push_back(ptree::value_type("", inputTree));
    }
    transactionTree.add_child("inputs", inputsTree);

    for(const auto &output : b.transaction.outputs) {
      ptree outputTree;
      outputTree.put("amount", output.amount);
      outputTree.put("key", "0x" + hashToHexStr(output.key));
      outputsTree.push_back(ptree::value_type("", outputTree));
    }
    transactionTree.add_child("outputs", outputsTree);

    blockTree.put("previous", "0x" + hashToHexStr(b.previous));
    blockTree.put("nonce", b.nonce);
    blockTree.put("height", b.height);
    blockTree.add_child("transaction", transactionTree);
    pt.add_child(std::to_string(b.height), blockTree);
  }

  std::ostringstream oss;
  boost::property_tree::json_parser::write_json(oss, pt);

  return oss.str();
}