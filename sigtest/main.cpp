//
// Created by artem on 24.08.20.
//
#include <sodium.h>
#include <iostream>
#include <boost/algorithm/hex.hpp>

template <class T>
std::string arrayToHexStr(const T &h) {
  std::string rawData(sizeof(T), '\0');
  std::copy(h, h + sizeof(h) / sizeof(h[0]), rawData.begin());
  std::string hexStr = boost::algorithm::hex(rawData);
  return hexStr;
}

unsigned char* hexStrToArray(const std::string &s, unsigned char* res) {
  std::string str = boost::algorithm::unhex(s);
  std::copy(str.begin(), str.end(), res);
  return res;
}

int main()
{
  if (sodium_init() < 0) {
    /* panic! the library couldn't be initialized, it is not safe to use */
    return 0;
  }
  std::string txHash = "F208279F9A039B0232660311D0723168E9BC07318C78D1FF6D1C0123A76BC150";
  #define HASH_SIZE_IN_BYTES 32
  unsigned char messageBuffer[HASH_SIZE_IN_BYTES];

  unsigned char pk[crypto_sign_PUBLICKEYBYTES];
  unsigned char sk[crypto_sign_SECRETKEYBYTES];
  //crypto_sign_keypair(pk, sk);
  hexStrToArray("D25F2E1496034197C17B595CAC3CFBDAA3D04C0A46C17F62F8DABA4B8415B053", pk);
  hexStrToArray("209EFEC81D4E08E875C3C8F939300003ECFACC4E75A75D8D615282D087032F35D25F2E1496034197C17B595CAC3CFBDAA3D04C0A46C17F62F8DABA4B8415B053", sk);

  std::cout << "public key: " << arrayToHexStr(pk) << std::endl;
  std::cout << "private key: " << arrayToHexStr(sk).substr(0, 64) << std::endl;

  unsigned char signed_message[crypto_sign_BYTES + HASH_SIZE_IN_BYTES];
  unsigned long long signed_message_len;

  crypto_sign(signed_message, &signed_message_len,
              hexStrToArray(txHash, messageBuffer), HASH_SIZE_IN_BYTES, sk);

  std::cout << "signature: " << arrayToHexStr(signed_message).substr(0, 128) << std::endl;

  unsigned char unsigned_message[HASH_SIZE_IN_BYTES];
  unsigned long long unsigned_message_len;
  if (crypto_sign_open(unsigned_message, &unsigned_message_len,
                       signed_message, signed_message_len, pk) != 0) {
    std::cout << "incorrect sig" << std::endl;
  }
  std::cout << "correct sig" << std::endl;
  std::cout << "original message: " << arrayToHexStr(unsigned_message) << std::endl;
  return 0;
}