#pragma once

#include <stdint.h>

class NonceDistribution {
  protected:
    int m_numAsics;
    uint32_t *m_distribution = nullptr;

  public:
    NonceDistribution();
    void init(int numAsics);
    void addShare(int asicNr);
    void toLog();
};
