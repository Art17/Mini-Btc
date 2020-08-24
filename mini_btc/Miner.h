//
// Created by artem on 14.08.20.
//

#ifndef SIMPLE_WEB_SERVER_MINER_H
#define SIMPLE_WEB_SERVER_MINER_H

#include <condition_variable>
#include <queue>
#include <thread>
#include <unordered_map>

#include "Blockchain.h"
#include "mini_btc/data_types/Utxo.h"
#include "mini_btc/data_types/data_types.h"
#include <sodium.h>

enum TransactionCheckResult { Valid,
                              AddedToMempool,
                              UnknownInputHash,
                              InvalidInputIndex,
                              AlreadySpentOutput,
                              InvalidOutputAmount,
                              InvalidSignature };

struct UTXOValue
{
  bool isSpent;
  PubKey publicKey;
  uint32_t amount;
};

class Miner {
public:
  Miner();
  ~Miner();
  TransactionCheckResult addTransaction(const Transaction &tx);

  static uint32_t getHashDifficulty(const Hash &hash);
  static uint32_t countZeros(uint8_t x);

protected:
  TransactionCheckResult checkInputs(const Transaction &, uint32_t* amount) const;
  TransactionCheckResult checkOutputs(const Transaction &, uint32_t* amount) const;
  TransactionCheckResult checkSig(const Hash &txHash, const PubKey& pubKey, const Signature& sig) const;

private:
  Blockchain *blockchain;
  std::unordered_map<UTXO, UTXOValue> utxoMap;
  std::queue<Transaction> mempool;
  std::thread miningThread;

  std::mutex m;
  std::condition_variable cv;

  bool isShuttingDown;

  void mineTransactions();
  void mineTransaction(const Transaction& tx);
  void markInputsAsSpent(const Transaction& tx);
};


#endif //SIMPLE_WEB_SERVER_MINER_H
