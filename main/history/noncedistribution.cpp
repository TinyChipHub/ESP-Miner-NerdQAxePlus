#include <math.h>
#include <pthread.h>
#include <stdint.h>

#include "esp_log.h"
#include "esp_timer.h" // Include esp_timer for esp_timer_get_time
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "global_state.h"
#include "noncedistribution.h"

#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
#pragma GCC diagnostic error "-Wmissing-prototypes"

static const char *TAG = "nonce_dist";

// define for wrapped access of psram
#define WRAP(a) ((a) & (HISTORY_MAX_SAMPLES - 1))


NonceDistribution::NonceDistribution() {
    // NOP
}

void NonceDistribution::init(int numAsics)
{
    m_numAsics = numAsics;
    if (m_numAsics) {
        m_distribution = (uint32_t *) calloc(m_numAsics, sizeof(uint32_t));
    }
}

void NonceDistribution::addShare(int asicNr) {
    if (m_distribution && asicNr < m_numAsics) {
        m_distribution[asicNr]++;
    }
}

void NonceDistribution::toLog()
{
    // this can happen if we don't have asics
    if (!m_distribution) {
        return;
    }

    char buffer[256];
    size_t offset = 0;

    // Iterate through each ASIC and append its count to the log message
    for (int i = 0; i < m_numAsics; i++) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%lu/", m_distribution[i]);
    }
    if (offset > 0) {
        buffer[offset - 1] = 0; // remove trailing slash
    }

    ESP_LOGI(TAG, "nonce distribution: %s", buffer);
}
