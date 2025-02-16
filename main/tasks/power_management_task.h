#pragma once

#include <pthread.h>
#include "boards/board.h"

class PowerManagementTask {
  protected:
    pthread_mutex_t m_mutex;
    uint16_t m_fanPerc;
    uint16_t m_fanRPM;
    float m_chipTempAvg;
    float m_vrTemp;
    float m_voltage;
    float m_power;
    float m_current;

    double automaticFanSpeed(Board *board, float chip_temp);

    void task();

  public:
    PowerManagementTask();

    // synchronized rebooting to now mess up i2c comms
    void restart();

    static void taskWrapper(void *pvParameters);

    float getPower()
    {
        return m_power;
    };
    float getVoltage()
    {
        return m_voltage;
    };
    float getCurrent()
    {
        return m_current;
    };
    float getAvgChipTemp()
    {
        return m_chipTempAvg;
    };
    float getVRTemp()
    {
        return m_vrTemp;
    };
    uint16_t getFanRPM()
    {
        return m_fanRPM;
    };
    uint16_t getFanPerc()
    {
        return m_fanPerc;
    };
};
