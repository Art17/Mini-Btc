//
// Created by artem on 15.08.20.
//
#include "Utxo.h"

bool UTXO::operator==(const UTXO &p) const {
  return txHash == p.txHash && index == p.index;
}
