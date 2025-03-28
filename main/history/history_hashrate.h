#pragma once

#include <stdint.h>
#include "./history_average.h"

class HistoryHashRate: public HistoryAverage {
  public:
    double m_avg = 0;
    double m_avgGh = 0;
    bool m_preliminary = true;

  public:
    HistoryHashRate(uint64_t timespan);

    float getGh()
    {
        return m_avgGh;
    };

    uint64_t getTimestamp()
    {
        return m_timestamp;
    };

    bool isPreliminary()
    {
        return m_preliminary;
    };
    void update();
};
