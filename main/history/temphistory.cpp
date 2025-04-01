#include <math.h>
#include <pthread.h>
#include <stdint.h>

#include "esp_log.h"
#include "esp_timer.h" // Include esp_timer for esp_timer_get_time
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "global_state.h"
#include "./history/history.h"
#include "./temphistory.h"

#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
#pragma GCC diagnostic error "-Wmissing-prototypes"

static const char *TAG = "temp_hist";

// define for wrapped access of psram
#define WRAP(a) ((a) & (HISTORY_MAX_SAMPLES - 1))

float TempHistory::getTempSample(int index)
{
    return m_temperatures[WRAP(index)];
}

double TempHistory::getCurrentTemp()
{
    return m_tempHist.getAvg();
}

void TempHistory::lock()
{
    pthread_mutex_lock(&m_mutex);
}

void TempHistory::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

bool TempHistory::isAvailable()
{
    return m_temperatures && BaseHistory::isAvailable();
}

TempHistory::TempHistory() : m_tempHist(10llu * 1000llu)
{
    // NOP
}

bool TempHistory::init(int num_asics)
{
    if (!BaseHistory::init(num_asics)) {
        return false;
    }
    m_temperatures = (float*) heap_caps_malloc(HISTORY_MAX_SAMPLES * sizeof(float), MALLOC_CAP_SPIRAM);

    return isAvailable();
}

void TempHistory::pushTemp(float temp, uint64_t timestamp)
{
    if (!isAvailable()) {
        ESP_LOGW(TAG, "PSRAM not initialized");
        return;
    }

    lock();
    m_temperatures[WRAP(m_numSamples)] = temp;
    BaseHistory::push(timestamp);
    m_numSamples++;
    unlock();

    update(m_tempHist);

//    ESP_LOGI(TAG, "hashrate: 10m:%.3fGH%c 1h:%.3fGH%c 1d:%.3fGH%c", m_avg10m.getGh(), preliminary_10m, m_avg1h.getGh(),
//             preliminary_1h, m_avg1d.getGh(), preliminary_1d);
}

void TempHistory::update(HistoryAverage &hist)
{
    // Catch up with the latest sample and update diffsum
    uint64_t lastTimestamp = 0;
    while (lastTimestamp = getTimestampSample(hist.m_lastSample), hist.m_lastSample + 1 < getNumSamples()) {
        hist.m_lastSample++;
        hist.m_sum += getTempSample(hist.m_lastSample);
    }

    // adjust the window on the older side
    // we move the lower window bound until the next sample would be out of
    // the desired timespan.
    while (hist.m_firstSample + 1 < hist.m_lastSample && lastTimestamp - getTimestampSample(hist.m_firstSample + 1) >= hist.m_timespan) {
        hist.m_sum -= getTempSample(hist.m_firstSample);
        hist.m_firstSample++;
    }

    uint64_t firstTimestamp = getTimestampSample(hist.m_firstSample);

    // Prevent division by zero
    if (lastTimestamp == firstTimestamp) {
        ESP_LOGW(TAG, "Timestamps are equal; cannot compute average.");
        return;
    }

    // Calculate the average hash rate
    uint64_t duration = (lastTimestamp - firstTimestamp);

    // clamp duration to a minimum value of avg->timespan
    duration = (hist.m_timespan > duration) ? hist.m_timespan : duration;

    hist.m_avg = (float) (hist.m_sum) / ((float) duration / 1.0e3);
    hist.m_timestamp = lastTimestamp;
}
