//
// Created by artem on 14.08.20.
//

#include "data_types.h"
#include "mini_btc/utils.h"
#include <algorithm>

Hash Input::calculateHash() const {
  std::string rawData(sizeof(Hash) + sizeof(uint32_t), '\0');
  std::copy(hash.begin(), hash.end(), rawData.begin());
  std::vector<uint8_t> indexBytes = uintToBytes(index);
  std::copy(indexBytes.begin(), indexBytes.end(), rawData.begin() + sizeof(Hash));

  return sha256bytes(rawData);
}

Hash Output::calculateHash() const {
  std::string rawData(sizeof(uint32_t) + sizeof(PubKey), '\0');
  std::vector<uint8_t> amountBytes = uintToBytes(amount);
  std::copy(amountBytes.begin(), amountBytes.end(), rawData.begin());
  std::copy(key.begin(), key.end(), rawData.begin() + sizeof(uint32_t));

  return sha256bytes(rawData);
}

Hash Transaction::calculateHash() const {
  std::string rawData((inputs.size() + outputs.size()) * sizeof(Hash), '\0');
  int index = 0;

  for(const Input &input : inputs) {
    Hash inputHash = input.calculateHash();
    std::copy(inputHash.begin(), inputHash.end(), rawData.begin() + index);
    index += sizeof(Hash);
  }
  for(const Output &output : outputs) {
    Hash outputHash = output.calculateHash();
    std::copy(outputHash.begin(), outputHash.end(), rawData.begin() + index);
    index += sizeof(Hash);
  }

  return sha256bytes(rawData);
}

void Block::updateRawDataNonce(std::string& rawData) const {
  auto rawDataItr = rawData.begin();

  rawDataItr += sizeof(Hash);

  std::vector<uint8_t> nonceBytes = uintToBytes(nonce);
  std::copy(nonceBytes.begin(), nonceBytes.end(), rawDataItr);
}

std::string Block::getRawData() const {
  std::string rawData(sizeof(Hash) + sizeof(uint64_t) + sizeof(uint32_t) + sizeof(Hash), '\0');
  auto rawDataItr = rawData.begin();

  std::copy(previous.begin(), previous.end(), rawDataItr);
  rawDataItr += sizeof(Hash);

  std::vector<uint8_t> nonceBytes = uintToBytes(nonce);
  std::copy(nonceBytes.begin(), nonceBytes.end(), rawDataItr);
  rawDataItr += sizeof(uint64_t);

  std::vector<uint8_t> heightBytes = uintToBytes(height);
  std::copy(heightBytes.begin(), heightBytes.end(), rawDataItr);
  rawDataItr += sizeof(uint32_t);

  Hash transactionHash = transaction.calculateHash();
  std::copy(transactionHash.begin(), transactionHash.end(), rawDataItr);

  return rawData;
}
