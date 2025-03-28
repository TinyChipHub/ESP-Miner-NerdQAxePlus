#pragma once

#include <stdint.h>

#include "./basehistory.h"
#include "./history_hashrate.h"
#include "./noncedistribution.h"

// 128k samples should be enough^^
// must be power of two
#define HISTORY_MAX_SAMPLES 0x20000

class TempHistory : public BaseHistory {
  protected:
    float *m_temperatures = nullptr;

    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;

    HistoryAverage m_tempHist;

    void update(HistoryAverage &hist);

  public:
    TempHistory();
    bool init(int numAsics);
    bool isAvailable();
    void pushTemp(float temp, uint64_t timestamp);

    void lock();
    void unlock();

    float getTempSample(int index);

    double getCurrentTemp();
};
