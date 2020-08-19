//
// Created by artem on 15.08.20.
//

#ifndef SIMPLE_WEB_SERVER_UTXO_H
#define SIMPLE_WEB_SERVER_UTXO_H

#include "data_types.h"

struct UTXO {
  Hash txHash;
  uint32_t index;

  bool operator==(const UTXO &p) const;
};

namespace std {

  template <>
  struct hash<Hash> {
    std::size_t operator()(const Hash &h) const {
      std::size_t res = 0;
      for(const auto &i : h) {
        res ^= std::hash<uint8_t>()(i);
      }

      return res;
    }
  };

  template <>
  struct hash<UTXO> {
    std::size_t operator()(const UTXO &k) const {
      std::size_t h1 = std::hash<Hash>()(k.txHash);
      std::size_t h2 = std::hash<uint32_t>()(k.index);

      return h1 ^ h2;
    }
  };
} // namespace std

#endif //SIMPLE_WEB_SERVER_UTXO_H
