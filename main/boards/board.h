#pragma once

#include "../displays/images/themes/themes.h"
#include "asic.h"
#include "bm1368.h"
#include "nvs_config.h"

// we need packed here (for memcmp)
struct __attribute__((packed)) PidSettings {
    uint16_t targetTemp;
    uint16_t p;
    uint16_t i;
    uint16_t d;
};

class Board {
  protected:
    // general board information
    const char *m_deviceModel;
    int m_version;
    const char *m_asicModel;
    const char *m_miningAgent;
    int m_asicCount;
    int m_chipsDetected = 0;
    int m_numTempSensors = 0;

    PidSettings m_pidSettings;

    // asic settings
    int m_asicJobIntervalMs;
    int m_asicFrequency;
    int m_asicVoltageMillis;

    // default settings
    int m_defaultAsicFrequency;
    int m_defaultAsicVoltageMillis;


    // asic difficulty settings
    uint32_t m_asicMinDifficulty;
    uint32_t m_asicMaxDifficulty;

    // Voltage regulator max temperature
    float m_vr_maxTemp = 0.0;

    // fans
    bool m_fanInvertPolarity;
    float m_fanPerc;

    // flip screen
    bool m_flipScreen;

    // max power settings
    float m_maxPin;
    float m_minPin;
    float m_maxVin;
    float m_minVin;

    // automatic fan control settings
    float m_afcMinTemp;
    float m_afcMinFanSpeed;
    float m_afcMaxTemp;

    // display m_theme
    Theme *m_theme = nullptr;

    Asic *m_asics = nullptr;

    bool m_isInitialized = false;

  public:
    Board();

    virtual bool initBoard() = 0;
    virtual bool initAsics() = 0;

    void loadSettings();
    const char *getDeviceModel();
    const char *getMiningAgent();
    int getVersion();
    const char *getAsicModel();
    int getAsicCount();
    int getAsicJobIntervalMs();
    uint32_t getInitialASICDifficulty();

    // abstract common methos
    virtual bool setVoltage(float core_voltage) = 0;

    virtual void setFanSpeed(float perc) = 0;
    virtual void getFanSpeed(uint16_t *rpm) = 0;

    virtual float getTemperature(int index) = 0;
    virtual float getVRTemp() = 0;
    virtual bool isPIDAvailable() = 0;

    virtual float getVin() = 0;
    virtual float getIin() = 0;
    virtual float getPin() = 0;
    virtual float getVout() = 0;
    virtual float getIout() = 0;
    virtual float getPout() = 0;

    virtual void requestBuckTelemtry() = 0;

    virtual float automaticFanSpeed(float temp);

    virtual void shutdown() = 0;

    virtual bool getPSUFault()
    {
        return false;
    }

    virtual bool selfTest();

    Theme *getTheme()
    {
        return m_theme;
    };

    uint32_t getAsicMaxDifficulty()
    {
        return m_asicMaxDifficulty;
    };
    uint32_t getAsicMinDifficulty()
    {
        return m_asicMinDifficulty;
    };

    bool isInitialized()
    {
        return m_isInitialized;
    };

    virtual Asic *getAsics()
    {
        return m_isInitialized ? m_asics : nullptr;
    }

    int getAsicVoltageMillis()
    {
        return m_asicVoltageMillis;
    }

    int getAsicFrequency()
    {
        return m_asicFrequency;
    }

    int getDefaultAsicVoltageMillis()
    {
        return m_defaultAsicVoltageMillis;
    }

    int getDefaultAsicFrequency()
    {
        return m_defaultAsicFrequency;
    }

    float getMinPin()
    {
        return m_minPin;
    }

    float getMaxPin()
    {
        return m_maxPin;
    }

    float getMinVin()
    {
        return m_minVin;
    }

    float getMaxVin()
    {
        return m_maxVin;
    }

    float getVrMaxTemp()
    {
        return m_vr_maxTemp;
    }

    int getNumTempSensors()
    {
        return m_numTempSensors;
    }

    bool isFlipScreenEnabled()
    {
        return m_flipScreen;
    }

    bool isInvertFanPolarityEnabled()
    {
        return m_fanInvertPolarity;
    }

    PidSettings *getPidSettings() {
        return &m_pidSettings;
    }

};
