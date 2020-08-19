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

enum TransactionCheckResult { Valid,
                              AddedToMempool,
                              UnknownInputHash,
                              InvalidInputIndex,
                              AlreadySpentOutput,
                              InvalidOutputAmount,
                              InvalidSignature };

class Miner {
public:
  Miner();
  ~Miner();
  TransactionCheckResult addTransaction(const Transaction &tx);

  static uint32_t getHashDifficulty(const Hash &hash);
  static uint32_t countZeros(uint8_t x);

protected:
  TransactionCheckResult checkInputs(const Transaction &) const;
  TransactionCheckResult checkOutputs(const Transaction &) const;
  TransactionCheckResult checkSigs(const Transaction &) const;

private:
  Blockchain *blockchain;
  std::unordered_map<UTXO, bool> utxoMap;
  std::queue<Transaction> mempool;
  std::thread miningThread;

  std::mutex m;
  std::condition_variable cv;

  bool isShuttingDown;

  void mineTransactions();
  void mineTransaction(const Transaction& tx);
  void invalidateUtxo(const Transaction& tx);
};


#endif //SIMPLE_WEB_SERVER_MINER_H