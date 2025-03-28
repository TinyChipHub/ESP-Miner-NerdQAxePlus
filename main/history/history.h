#pragma once

#include <stdint.h>
#include <pthread.h>

#include "./basehistory.h"
#include "./history_hashrate.h"
#include "./noncedistribution.h"

// 128k samples should be enough^^
// must be power of two
#define HISTORY_MAX_SAMPLES 0x20000

class History : public BaseHistory {
  protected:
    uint32_t *m_shares = nullptr;
    float *m_hashrate10m = nullptr;
    float *m_hashrate1h = nullptr;
    float *m_hashrate1d = nullptr;

    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;

    HistoryHashRate m_avg10m;
    HistoryHashRate m_avg1h;
    HistoryHashRate m_avg1d;
    NonceDistribution m_distribution;

    void update(HistoryHashRate &hist);

  public:
    History();
    bool init(int numAsics);
    bool isAvailable();
    void pushShare(uint32_t diff, uint64_t timestamp, int asic_nr);

    void lock();
    void unlock();

    float getHashrate10mSample(int index);
    float getHashrate1hSample(int index);
    float getHashrate1dSample(int index);

    double getCurrentHashrate10m();
    double getCurrentHashrate1h();
    double getCurrentHashrate1d();
    uint32_t getShareSample(int index);
};
