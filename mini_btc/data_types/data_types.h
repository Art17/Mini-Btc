//
// Created by artem on 13.08.20.
//

#ifndef SIMPLE_WEB_SERVER_DATA_TYPES_H
#define SIMPLE_WEB_SERVER_DATA_TYPES_H

#include <array>
#include <vector>

typedef std::array<uint8_t, 32> Hash;
typedef std::array<uint8_t, 64> Signature;
typedef std::array<uint8_t, 32> PubKey;

struct Input {
  Hash hash;
  uint32_t index;
  Signature sig;

  Hash calculateHash() const;
};

struct Output {
  uint32_t amount;
  PubKey key;

  Hash calculateHash() const;
};

struct Transaction {
  std::vector<Input> inputs;
  std::vector<Output> outputs;

  Hash calculateHash() const;
};

struct Block {
  Hash previous;
  uint64_t nonce;
  uint32_t height;
  Transaction transaction;

  void updateRawDataNonce(std::string& rawData) const;
  std::string getRawData() const;
};

#endif //SIMPLE_WEB_SERVER_DATA_TYPES_H
