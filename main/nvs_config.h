#pragma once

#include <stdint.h>

// Max length 15

#define NVS_CONFIG_WIFI_SSID "wifissid"
#define NVS_CONFIG_WIFI_PASS "wifipass"
#define NVS_CONFIG_HOSTNAME "hostname"
#define NVS_CONFIG_STRATUM_URL "stratumurl"
#define NVS_CONFIG_STRATUM_PORT "stratumport"
#define NVS_CONFIG_STRATUM_USER "stratumuser"
#define NVS_CONFIG_STRATUM_PASS "stratumpass"
#define NVS_CONFIG_STRATUM_FALLBACK_URL "fbstratumurl"
#define NVS_CONFIG_STRATUM_FALLBACK_PORT "fbstratumport"
#define NVS_CONFIG_STRATUM_FALLBACK_USER "fbstratumuser"
#define NVS_CONFIG_STRATUM_FALLBACK_PASS "fbstratumpass"
#define NVS_CONFIG_ASIC_FREQ "asicfrequency"
#define NVS_CONFIG_ASIC_VOLTAGE "asicvoltage"
#define NVS_CONFIG_ASIC_JOB_INTERVAL "asicjobinterval"
#define NVS_CONFIG_FLIP_SCREEN "flipscreen"
#define NVS_CONFIG_INVERT_SCREEN "invertscreen"
#define NVS_CONFIG_INVERT_FAN_POLARITY "invertfanpol"
#define NVS_CONFIG_AUTO_FAN_SPEED "autofanspeed"
#define NVS_CONFIG_FAN_SPEED "fanspeed"
#define NVS_CONFIG_BEST_DIFF "bestdiff"
#define NVS_CONFIG_SELF_TEST "selftest"
#define NVS_CONFIG_AUTO_SCREEN_OFF "autoscreenoff"
#define NVS_CONFIG_OVERHEAT_TEMP "overheat_temp"

#define NVS_CONFIG_INFLUX_ENABLE "influx_enable"
#define NVS_CONFIG_INFLUX_URL "influx_url"
#define NVS_CONFIG_INFLUX_TOKEN "influx_token"
#define NVS_CONFIG_INFLUX_PORT "influx_port"
#define NVS_CONFIG_INFLUX_BUCKET "influx_bucket"
#define NVS_CONFIG_INFLUX_ORG "influx_org"
#define NVS_CONFIG_INFLUX_PREFIX "influx_prefix"

#define NVS_CONFIG_SWARM "swarmconfig"

class Config {
  private:
    const char *m_WifiSSID = nullptr;
    const char *m_WifiPass = nullptr;
    const char *m_Hostname = nullptr;
    const char *m_StratumURL = nullptr;
    uint16_t m_StratumPort;
    const char *m_StratumUser = nullptr;
    const char *m_StratumPass = nullptr;
    const char *m_StratumFallbackURL = nullptr;
    uint16_t m_StratumFallbackPort;
    const char *m_StratumFallbackUser = nullptr;
    const char *m_StratumFallbackPass = nullptr;
    int m_AsicFreq;
    int m_AsicVoltage;
    int m_AsicJobInterval;
    bool m_FlipScreen;
    bool m_InvertScreen;
    bool m_InvertFanPolarity;
    bool m_AutoFanSpeed;
    int m_FanSpeed;
    float m_BestDiff;
    bool m_SelfTest;
    bool m_AutoScreenOff;
    int m_OverheatTemp;

    bool m_InfluxEnable;
    const char *m_InfluxURL = nullptr;
    int m_InfluxPort;
    const char *m_InfluxToken = nullptr;
    const char *m_InfluxBucket = nullptr;
    const char *m_InfluxOrg = nullptr;
    const char *m_InfluxPrefix = nullptr;

    const char *m_SwarmConfig = nullptr;

    void free();

    char *nvs_config_get_string(const char *key, const char *default_value);
    void nvs_config_set_string(const char *key, const char *default_value);
    uint16_t nvs_config_get_u16(const char *key, const uint16_t default_value);
    void nvs_config_set_u16(const char *key, const uint16_t value);
    uint64_t nvs_config_get_u64(const char *key, const uint64_t default_value);
    void nvs_config_set_u64(const char *key, const uint64_t value);

  public:
    // Constructor
    Config();

    void loadConfig();

    // Getters
    const char *getWifiSSID() const { return m_WifiSSID; }
    const char *getWifiPass() const { return m_WifiPass; }
    const char *getHostname() const { return m_Hostname; }
    const char *getStratumURL() const { return m_StratumURL; }
    uint16_t getStratumPort() const { return m_StratumPort; }
    const char *getStratumUser() const { return m_StratumUser; }
    const char *getStratumPass() const { return m_StratumPass; }
    const char *getStratumFallbackURL() const { return m_StratumFallbackURL; }
    uint16_t getStratumFallbackPort() const { return m_StratumFallbackPort; }
    const char *getStratumFallbackUser() const { return m_StratumFallbackUser; }
    const char *getStratumFallbackPass() const { return m_StratumFallbackPass; }
    int getAsicFreq() const { return m_AsicFreq; }
    int getAsicVoltage() const { return m_AsicVoltage; }
    int getAsicJobInterval() const { return m_AsicJobInterval; }
    bool isFlipScreen() const { return m_FlipScreen; }
    bool isInvertScreen() const { return m_InvertScreen; }
    bool isInvertFanPolarity() const { return m_InvertFanPolarity; }
    bool isAutoFanSpeed() const { return m_AutoFanSpeed; }
    int getFanSpeed() const { return m_FanSpeed; }
    float getBestDiff() const { return m_BestDiff; }
    bool isSelfTest() const { return m_SelfTest; }
    bool isAutoScreenOff() const { return m_AutoScreenOff; }
    int getOverheatTemp() const { return m_OverheatTemp; }
    bool isInfluxEnabled() const { return m_InfluxEnable; }
    const char *getInfluxURL() const { return m_InfluxURL; }
    const char *getInfluxToken() const { return m_InfluxToken; }
    int getInfluxPort() const { return m_InfluxPort; }
    const char *getInfluxBucket() const { return m_InfluxBucket; }
    const char *getInfluxOrg() const { return m_InfluxOrg; }
    const char *getInfluxPrefix() const { return m_InfluxPrefix; }
    const char *getSwarmConfig() const { return m_SwarmConfig; }

    // Setters (values stored in NVS immediately)
    void setWifiSSID(const char *value) { nvs_config_set_string(NVS_CONFIG_WIFI_SSID, value); }
    void setWifiPass(const char *value) { nvs_config_set_string(NVS_CONFIG_WIFI_PASS, value); }
    void setHostname(const char *value) { nvs_config_set_string(NVS_CONFIG_HOSTNAME, value); }
    void setStratumURL(const char *value) { nvs_config_set_string(NVS_CONFIG_STRATUM_URL, value); }
    void setStratumPort(uint16_t value) { nvs_config_set_u16(NVS_CONFIG_STRATUM_PORT, value); }
    void setStratumUser(const char *value) { nvs_config_set_string(NVS_CONFIG_STRATUM_USER, value); }
    void setStratumPass(const char *value) { nvs_config_set_string(NVS_CONFIG_STRATUM_PASS, value); }
    void setStratumFallbackURL(const char *value) { nvs_config_set_string(NVS_CONFIG_STRATUM_FALLBACK_URL, value); }
    void setStratumFallbackPort(uint16_t value) { nvs_config_set_u16(NVS_CONFIG_STRATUM_FALLBACK_PORT, value); }
    void setStratumFallbackUser(const char *value) { nvs_config_set_string(NVS_CONFIG_STRATUM_FALLBACK_USER, value); }
    void setStratumFallbackPass(const char *value) { nvs_config_set_string(NVS_CONFIG_STRATUM_FALLBACK_PASS, value); }
    void setAsicFreq(int value) { nvs_config_set_u16(NVS_CONFIG_ASIC_FREQ, value); }
    void setAsicVoltage(int value) { nvs_config_set_u16(NVS_CONFIG_ASIC_VOLTAGE, value); }
    void setAsicJobInterval(int value) { nvs_config_set_u16(NVS_CONFIG_ASIC_JOB_INTERVAL, value); }
    void setFlipScreen(bool value) { nvs_config_set_u16(NVS_CONFIG_FLIP_SCREEN, value ? 1 : 0); }
    void setInvertScreen(bool value) { nvs_config_set_u16(NVS_CONFIG_INVERT_SCREEN, value ? 1 : 0); }
    void setInvertFanPolarity(bool value) { nvs_config_set_u16(NVS_CONFIG_INVERT_FAN_POLARITY, value ? 1 : 0); }
    void setAutoFanSpeed(bool value) { nvs_config_set_u16(NVS_CONFIG_AUTO_FAN_SPEED, value ? 1 : 0); }
    void setFanSpeed(int value) { nvs_config_set_u16(NVS_CONFIG_FAN_SPEED, value); }
    void setBestDiff(float value) { nvs_config_set_u64(NVS_CONFIG_BEST_DIFF, (uint64_t)value); }
    void setSelfTest(bool value) { nvs_config_set_u16(NVS_CONFIG_SELF_TEST, value ? 1 : 0); }
    void setOverheatTemp(int value) { nvs_config_set_u16(NVS_CONFIG_OVERHEAT_TEMP, value); }
    void setAutoScreenOff(bool value) { nvs_config_set_u16(NVS_CONFIG_AUTO_SCREEN_OFF, value ? 1 : 0); }
    void setInfluxEnable(bool value) { nvs_config_set_u16(NVS_CONFIG_INFLUX_ENABLE, value ? 1 : 0); }
    void setInfluxURL(const char *value) { nvs_config_set_string(NVS_CONFIG_INFLUX_URL, value); }
    void setInfluxToken(const char *value) { nvs_config_set_string(NVS_CONFIG_INFLUX_TOKEN, value); }
    void setInfluxPort(int value) { nvs_config_set_u16(NVS_CONFIG_INFLUX_PORT, value); }
    void setInfluxBucket(const char *value) { nvs_config_set_string(NVS_CONFIG_INFLUX_BUCKET, value); }
    void setInfluxOrg(const char *value) { nvs_config_set_string(NVS_CONFIG_INFLUX_ORG, value); }
    void setInfluxPrefix(const char *value) { nvs_config_set_string(NVS_CONFIG_INFLUX_PREFIX, value); }
    void setSwarmConfig(const char *value) { nvs_config_set_string(NVS_CONFIG_SWARM, value); }
};

