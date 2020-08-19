//
// Created by artem on 13.08.20.
//

#ifndef SIMPLE_WEB_SERVER_BLOCKCHAIN_H
#define SIMPLE_WEB_SERVER_BLOCKCHAIN_H

#include "mini_btc/data_types/data_types.h"

class Blockchain {

public:
  void addBlock(const Block &block);
  Hash getLastBlockHash();
  uint32_t getLastBlockHeight() const;
  uint32_t getDifficulty() const;
  void updateDifficulty();

  Blockchain(Blockchain &other) = delete;
  void operator=(const Blockchain &) = delete;
  static Blockchain *getInstance();

  std::string jsonify() const;

private:
  Blockchain();
  Hash lastBlockHash;
  bool isLastBlockHashValid;

  static Blockchain *blockchain;

  std::vector<Block> blocks;
  uint32_t difficulty;
};

#endif //SIMPLE_WEB_SERVER_BLOCKCHAIN_H
