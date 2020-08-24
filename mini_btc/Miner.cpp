//
// Created by artem on 14.08.20.
//

#include <iostream>

#include "Miner.h"
#include "Blockchain.h"
#include "utils.h"

Miner::Miner():
   blockchain(Blockchain::getInstance()),
   miningThread(&Miner::mineTransactions, this),
   isShuttingDown(false)
{
}

Miner::~Miner()
{
  isShuttingDown = true;
  miningThread.join();
}

TransactionCheckResult Miner::addTransaction(const Transaction &tx) {
  TransactionCheckResult txCheckStatus;
  txCheckStatus = checkInputs(tx);
  if(txCheckStatus != Valid)
    return txCheckStatus;
  txCheckStatus = checkOutputs(tx);
  if(txCheckStatus != Valid)
    return txCheckStatus;

  mempool.push(tx);
  cv.notify_all();

  return AddedToMempool;
}

void Miner::mineTransactions()
{
  std::unique_lock<std::mutex> lk(m);

  while(!isShuttingDown)
  {
    while (!mempool.empty())
    {
      Transaction tx = mempool.front();
      mineTransaction(tx);
      mempool.pop();
    }
    if(!isShuttingDown)
      cv.wait(lk);
  }
}

void Miner::mineTransaction(const Transaction &tx)
{
  Hash txHash = tx.calculateHash();
  Block block;
  block.previous = blockchain->getLastBlockHash();
  block.height = blockchain->getLastBlockHeight();
  block.transaction = tx;
  block.nonce = 0;
  uint32_t targetDifficulty = blockchain->getDifficulty();

  std::string blockRawData = block.getRawData();
  Hash blockHash = sha256bytes(blockRawData);

  while(getHashDifficulty(blockHash) < targetDifficulty) {
    block.nonce += 1;
    block.updateRawDataNonce(blockRawData);
    blockHash = sha256bytes(blockRawData);
  }

  uint32_t outputIndex = 0;
  for(const auto &output : tx.outputs) {
    UTXO utxo{txHash, outputIndex++};
    utxoMap[utxo] = UTXOValue{false, output.key};
  }

  invalidateUtxo(tx);
  blockchain->addBlock(block);
  std::cout << "Mined block " << block.height << " with hash " << "0x" << hashToHexStr(blockHash) << " (nonce: " << block.nonce << " )"
            << " transaction 0x" << hashToHexStr(txHash) << std::endl;
}

void Miner::invalidateUtxo(const Transaction& tx)
{
  for(const auto &input : tx.inputs) {
    UTXO utxo{input.hash, input.index};
    utxoMap[utxo].isSpent = true;
  }

}

TransactionCheckResult Miner::checkInputs(const Transaction &tx) const {
  std::unordered_map<UTXO, bool> tempUtxo;
  for(const auto &input : tx.inputs) {
    UTXO utxo{input.hash, input.index};
    const auto &utxoItr = utxoMap.find(utxo);
    if(utxoItr == utxoMap.end())
      return UnknownInputHash;
    else {
      if(utxoItr->second.isSpent)
        return AlreadySpentOutput;
      else
      {
        const auto &tempUtxoItr = tempUtxo.find(utxo);
        if(tempUtxoItr != tempUtxo.end() && tempUtxoItr->second)
          return AlreadySpentOutput;

        TransactionCheckResult txCheckStatus = checkSig(tx.calculateHash(), utxoItr->second.publicKey, input.sig);
        if(txCheckStatus != Valid)
          return txCheckStatus;

        tempUtxo[utxo] = true;
      }
    }
  }
  return Valid;
}

TransactionCheckResult Miner::checkOutputs(const Transaction &tx) const  {
  for(const auto &output : tx.outputs) {
    if(output.amount == 0)
      return InvalidOutputAmount;
  }
  return Valid;
}

TransactionCheckResult Miner::checkSig(const Hash &txHash, const PubKey& pubKey, const Signature& sig) const  {
  unsigned char unsigned_message[txHash.size()];
  unsigned long long unsigned_message_len;

  unsigned char signed_message[sig.size() + txHash.size()];
  unsigned long long signed_message_len = sig.size() + txHash.size();
  std::copy(sig.begin(), sig.end(), signed_message);
  std::copy(txHash.begin(), txHash.end(), signed_message + sig.size());

  if (crypto_sign_open(unsigned_message, &unsigned_message_len,
                       signed_message, signed_message_len, pubKey.begin()) != 0) {
    return InvalidSignature;
  }

  return Valid;
}

uint32_t Miner::getHashDifficulty(const Hash &hash) {
  uint32_t difficulty = 0;
  const int bitsInUint8 = 8;
  for(const auto &h : hash) {
    uint32_t zeros = countZeros(h);
    difficulty += zeros;
    if(zeros != bitsInUint8)
      break;
  }
  return difficulty;
}

uint32_t Miner::countZeros(uint8_t x) {
  unsigned y;
  uint32_t n = 8;
  y = x >> 4;
  if(y != 0) {
    n = n - 4;
    x = y;
  }
  y = x >> 2;
  if(y != 0) {
    n = n - 2;
    x = y;
  }
  y = x >> 1;
  if(y != 0)
    return n - 2;
  return n - x;
}