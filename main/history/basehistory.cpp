#include <math.h>
#include <pthread.h>
#include <stdint.h>

#include "esp_log.h"
#include "esp_timer.h" // Include esp_timer for esp_timer_get_time
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "global_state.h"
#include "basehistory.h"

#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
#pragma GCC diagnostic error "-Wmissing-prototypes"

static const char *TAG = "base_history";

// define for wrapped access of psram
#define WRAP(a) ((a) & (HISTORY_MAX_SAMPLES - 1))


bool BaseHistory::isAvailable()
{
    return m_timestamps;
}

BaseHistory::BaseHistory()
{
    // NOP
}

bool BaseHistory::init(int num_asics)
{
    m_timestamps = (uint64_t *) heap_caps_malloc(HISTORY_MAX_SAMPLES * sizeof(uint64_t), MALLOC_CAP_SPIRAM);
    return isAvailable();
}


uint64_t BaseHistory::getTimestampSample(int index)
{
    return m_timestamps[WRAP(index)];
}

void BaseHistory::getTimestamps(uint64_t *first, uint64_t *last, int *num_samples)
{
    int lowest_index = (m_numSamples - HISTORY_MAX_SAMPLES < 0) ? 0 : m_numSamples - HISTORY_MAX_SAMPLES;
    int highest_index = m_numSamples - 1;

    int _num_samples = highest_index - lowest_index + 1;

    uint64_t first_timestamp = (_num_samples) ? getTimestampSample(lowest_index) : 0;
    uint64_t last_timestamp = (_num_samples) ? getTimestampSample(highest_index) : 0;

    *first = first_timestamp;
    *last = last_timestamp;
    *num_samples = _num_samples;
}

uint64_t BaseHistory::getCurrentTimestamp()
{
    // all timestamps are equal
    return m_currentTimestamp;
}

void BaseHistory::push(uint64_t timestamp)
{
    if (!isAvailable()) {
        ESP_LOGW(TAG, "PSRAM not initialized");
        return;
    }

    m_timestamps[WRAP(m_numSamples)] = timestamp;
}


// successive approximation in a wrapped ring buffer with
// monotonic/unwrapped write pointer :woozy:
int BaseHistory::searchNearestTimestamp(int64_t timestamp)
{
    // get index of the first sample, clamp to min 0
    int lowest_index = (m_numSamples - HISTORY_MAX_SAMPLES < 0) ? 0 : m_numSamples - HISTORY_MAX_SAMPLES;

    // last sample
    int highest_index = m_numSamples - 1;

    ESP_LOGD(TAG, "lowest_index: %d highest_index: %d", lowest_index, highest_index);

    int current = 0;
    int num_elements = 0;

    while (current = (highest_index + lowest_index) / 2, num_elements = highest_index - lowest_index + 1, num_elements > 1) {
        // Get timestamp at the current index, wrapping as necessary
        uint64_t stored_timestamp = getTimestampSample(current);
        ESP_LOGD(TAG, "current %d num_elements %d stored_timestamp %llu wrapped-current %d", current, num_elements,
                 stored_timestamp, WRAP(current));

        if ((int64_t) stored_timestamp > timestamp) {
            // If timestamp is too large, search lower
            highest_index = current - 1; // Narrow the search to the lower half
        } else if ((int64_t) stored_timestamp < timestamp) {
            // If timestamp is too small, search higher
            lowest_index = current + 1; // Narrow the search to the upper half
        } else {
            // Exact match found
            return current;
        }
    }

    ESP_LOGD(TAG, "current return %d", current);

    if (current < 0 || current >= m_numSamples) {
        return -1;
    }

    return current;
}
