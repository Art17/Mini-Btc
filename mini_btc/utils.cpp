//
// Created by artem on 14.08.20.
//

#include <boost/algorithm/hex.hpp>
#include <string>

#include "mini_btc/data_types/data_types.h"

#ifdef HAVE_OPENSSL
#include "crypto.hpp"
#endif

#include "utils.h"


void remove0x(std::string &s) {
  if(s.rfind("0x", 0) == 0)
    s.erase(0, 2);
}

Hash sha256bytes(const std::string &strBytes) {
  std::string hashRes = SimpleWeb::Crypto::Crypto::sha256(strBytes);

  Hash res;
  std::copy(hashRes.begin(), hashRes.end(), res.begin());
  return res;
}

std::string sigToHexStr(const Signature &s) {
  return objToHexStr(s);
}

std::string hashToHexStr(const Hash &h) {
  return objToHexStr(h);
}

Signature hexStrToSig(const std::string &s) {
  return hexStrToObj<Signature>(s);
}

Hash hexStrToHash(const std::string &s) {
  return hexStrToObj<Hash>(s);
}
