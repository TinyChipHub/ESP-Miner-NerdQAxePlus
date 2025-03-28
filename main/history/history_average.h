#pragma once

#include <stdint.h>

class HistoryAverage {
  public:
    float m_sum = 0.0f;
    float m_avg = 0.0f;

    int m_firstSample = 0;
    int m_lastSample = 0;
    uint64_t m_timespan = 0;
    uint64_t m_diffSum = 0;
    uint64_t m_timestamp = 0;

  public:
    HistoryAverage(uint64_t timespan);

    uint64_t getTimestamp()
    {
        return m_timestamp;
    };

    float getAvg()
    {
        return m_avg;
    }

};
