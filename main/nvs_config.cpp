#include <string.h>

#include "esp_log.h"
#include "nvs.h"
#include "nvs_config.h"

#define NVS_CONFIG_NAMESPACE "main"

static const char *TAG = "nvs_config";

void safeFree(const char*& ptr) {
    if (ptr) {
        ::free(const_cast<char*>(ptr));
        ptr = nullptr;
    }
}

char *CONFIG::nvs_config_get_string(const char *key, const char *default_value)
{
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(NVS_CONFIG_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return strdup(default_value);
    }

    size_t size = 0;
    err = nvs_get_str(handle, key, NULL, &size);

    if (err != ESP_OK) {
        nvs_close(handle);
        return strdup(default_value);
    }

    char *out = (char *) malloc(size);
    err = nvs_get_str(handle, key, out, &size);

    if (err != ESP_OK) {
        free(out);
        nvs_close(handle);
        return strdup(default_value);
    }

    nvs_close(handle);
    return out;
}

void CONFIG::nvs_config_set_string(const char *key, const char *value)
{

    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(NVS_CONFIG_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Could not open nvs");
        return;
    }

    err = nvs_set_str(handle, key, value);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Could not write nvs key: %s, value: %s", key, value);
    }

    nvs_close(handle);
}

uint16_t CONFIG::nvs_config_get_u16(const char *key, const uint16_t default_value)
{
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(NVS_CONFIG_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return default_value;
    }

    uint16_t out;
    err = nvs_get_u16(handle, key, &out);
    nvs_close(handle);

    if (err != ESP_OK) {
        return default_value;
    }
    return out;
}

void CONFIG::nvs_config_set_u16(const char *key, const uint16_t value)
{

    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(NVS_CONFIG_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Could not open nvs");
        return;
    }

    err = nvs_set_u16(handle, key, value);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Could not write nvs key: %s, value: %u", key, value);
    }

    nvs_close(handle);
}

uint64_t CONFIG::nvs_config_get_u64(const char *key, const uint64_t default_value)
{
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(NVS_CONFIG_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return default_value;
    }

    uint64_t out;
    err = nvs_get_u64(handle, key, &out);

    if (err != ESP_OK) {
        nvs_close(handle);
        return default_value;
    }

    nvs_close(handle);
    return out;
}

void CONFIG::nvs_config_set_u64(const char *key, const uint64_t value)
{

    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(NVS_CONFIG_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Could not open nvs");
        return;
    }

    err = nvs_set_u64(handle, key, value);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Could not write nvs key: %s, value: %llu", key, value);
    }
    nvs_close(handle);
}

Config::Config()
{
    loadConfig();
}

void Config::free() {
    safeFree(m_WifiSSID);
    safeFree(m_WifiPass);
    safeFree(m_Hostname);
    safeFree(m_StratumURL);
    safeFree(m_StratumUser);
    safeFree(m_StratumPass);
    safeFree(m_StratumFallbackURL);
    safeFree(m_StratumFallbackUser);
    safeFree(m_StratumFallbackPass);
    safeFree(m_InfluxURL);
    safeFree(m_InfluxToken);
    safeFree(m_InfluxBucket);
    safeFree(m_InfluxOrg);
    safeFree(m_InfluxPrefix);
    safeFree(m_SwarmConfig);
}

void Config::loadConfig() {
    free();

    // Assuming a function nvs_config_get_str() exists to fetch string values
    m_WifiSSID = nvs_config_get_str(NVS_CONFIG_WIFI_SSID, CONFIG_ESP_WIFI_SSID);
    m_WifiPass = nvs_config_get_str(NVS_CONFIG_WIFI_PASS, CONFIG_ESP_WIFI_PASSWORD);
    m_Hostname = nvs_config_get_str(NVS_CONFIG_HOSTNAME, "");
    m_StratumURL = nvs_config_get_str(NVS_CONFIG_STRATUM_URL, CONFIG_STRATUM_URL);
    m_StratumPort = nvs_config_get_u16(NVS_CONFIG_STRATUM_PORT, CONFIG_STRATUM_PORT);
    m_StratumUser = nvs_config_get_str(NVS_CONFIG_STRATUM_USER, CONFIG_STRATUM_USER);
    m_StratumPass = nvs_config_get_str(NVS_CONFIG_STRATUM_PASS, CONFIG_STRATUM_PW);
    m_StratumFallbackURL = nvs_config_get_str(NVS_CONFIG_STRATUM_FALLBACK_URL, CONFIG_STRATUM_FALLBACK_URL);
    m_StratumFallbackPort = nvs_config_get_u16(NVS_CONFIG_STRATUM_FALLBACK_PORT, CONFIG_STRATUM_FALLBACK_PORT);
    m_StratumFallbackUser = nvs_config_get_str(NVS_CONFIG_STRATUM_FALLBACK_USER, CONFIG_STRATUM_FALLBACK_USER);
    m_StratumFallbackPass = nvs_config_get_str(NVS_CONFIG_STRATUM_FALLBACK_PASS, CONFIG_STRATUM_FALLBACK_PW);

    // Assuming nvs_config_get_u16() for integer values
    m_AsicFreq = nvs_config_get_u16(NVS_CONFIG_ASIC_FREQ, CONFIG_ASIC_FREQUENCY);
    m_AsicVoltage = nvs_config_get_u16(NVS_CONFIG_ASIC_VOLTAGE, CONFIG_ASIC_VOLTAGE);
    m_AsicJobInterval = nvs_config_get_u16(NVS_CONFIG_ASIC_JOB_INTERVAL, 1500); // TODO

    m_FlipScreen = nvs_config_get_u16(NVS_CONFIG_FLIP_SCREEN, CONFIG_FLIP_SCREEN_VALUE);
    m_InvertScreen = nvs_config_get_u16(NVS_CONFIG_INVERT_SCREEN, 0); // clean up, is unused
    m_InvertFanPolarity = nvs_config_get_u16(NVS_CONFIG_INVERT_FAN_POLARITY, CONFIG_INVERT_POLARITY_VALUE);
    m_AutoFanSpeed = nvs_config_get_u16(NVS_CONFIG_AUTO_FAN_SPEED, CONFIG_AUTO_FAN_SPEED_VALUE);
    m_FanSpeed = nvs_config_get_u16(NVS_CONFIG_FAN_SPEED, CONFIG_FAN_SPEED);
    m_BestDiff = nvs_config_get_float(NVS_CONFIG_BEST_DIFF, 0.0f);
    m_SelfTest = nvs_config_get_u16(NVS_CONFIG_SELF_TEST, 0);
    m_AutoScreenOff = nvs_config_get_u16(NVS_CONFIG_AUTO_SCREEN_OFF, CONFIG_AUTO_SCREEN_OFF_VALUE);
    m_OverheatTemp = nvs_config_get_u16(NVS_CONFIG_OVERHEAT_TEMP, CONFIG_OVERHEAT_TEMP);

    m_InfluxEnable = nvs_config_get_u16(NVS_CONFIG_INFLUX_ENABLE, CONFIG_INFLUX_ENABLE_VALUE);
    m_InfluxURL = nvs_config_get_str(NVS_CONFIG_INFLUX_URL, CONFIG_INFLUX_URL);
    m_InfluxToken = nvs_config_get_str(NVS_CONFIG_INFLUX_TOKEN, CONFIG_INFLUX_TOKEN);
    m_InfluxPort = nvs_config_get_u16(NVS_CONFIG_INFLUX_PORT, CONFIG_INFLUX_PORT);
    m_InfluxBucket = nvs_config_get_str(NVS_CONFIG_INFLUX_BUCKET, CONFIG_INFLUX_BUCKET);
    m_InfluxOrg = nvs_config_get_str(NVS_CONFIG_INFLUX_ORG, CONFIG_INFLUX_ORG);
    m_InfluxPrefix = nvs_config_get_str(NVS_CONFIG_INFLUX_PREFIX, CONFIG_INFLUX_PREFIX);

    m_SwarmConfig = nvs_config_get_str(NVS_CONFIG_SWARM, "");
}

