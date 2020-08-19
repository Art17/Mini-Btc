//
// Created by artem on 14.08.20.
//

#ifndef SIMPLE_WEB_SERVER_UTILS_H
#define SIMPLE_WEB_SERVER_UTILS_H

#include <boost/algorithm/hex.hpp>
#include <string>

void remove0x(std::string &s);
Hash sha256bytes(const std::string &strBytes);

Hash hexStrToHash(const std::string &s);
Signature hexStrToSig(const std::string &s);

std::string hashToHexStr(const Hash &h);
std::string sigToHexStr(const Signature &s);

template <class T>
T hexStrToObj(const std::string &s) {
  std::string str = boost::algorithm::unhex(s);
  T obj;
  std::copy(str.begin(), str.end(), obj.begin());
  return obj;
}

template <class T>
std::string objToHexStr(const T &h) {
  std::string rawData(sizeof(T), '\0');
  std::copy(h.begin(), h.end(), rawData.begin());
  std::string hexStr = boost::algorithm::hex(rawData);
  return hexStr;
}

template <class T>
std::vector<uint8_t> uintToBytes(T paramInt) {
  size_t size = sizeof(T);
  std::vector<uint8_t> arrayOfByte(size);
  for(std::size_t i = 0; i < size; i++)
    arrayOfByte[size - 1 - i] = (paramInt >> (i * 8));
  return arrayOfByte;
}

#endif //SIMPLE_WEB_SERVER_UTILS_H
