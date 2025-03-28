#pragma once

#include <stdint.h>

class BaseHistory {
  protected:
    int m_numSamples = 0;
    uint64_t *m_timestamps = nullptr;
    uint64_t m_currentTimestamp = 0;

    virtual void push(uint64_t timestamp);

  public:
    BaseHistory();

    virtual bool init(int numAsics);
    virtual bool isAvailable();
    virtual void getTimestamps(uint64_t *first, uint64_t *last, int *num_samples);

    uint64_t getTimestampSample(int index);
    uint64_t getCurrentTimestamp(void);
    int searchNearestTimestamp(int64_t timestamp);

    int getNumSamples()
    {
        return m_numSamples;
    };
};
