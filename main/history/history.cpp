#include <math.h>
#include <pthread.h>
#include <stdint.h>

#include "esp_log.h"
#include "esp_timer.h" // Include esp_timer for esp_timer_get_time
#include "esp_psram.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"


#include "global_state.h"
#include "./history/history.h"

#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
#pragma GCC diagnostic error "-Wmissing-prototypes"

static const char *TAG = "history";

// define for wrapped access of psram
#define WRAP(a) ((a) & (HISTORY_MAX_SAMPLES - 1))

float History::getHashrate10mSample(int index)
{
    return m_hashrate10m[WRAP(index)];
}

float History::getHashrate1hSample(int index)
{
    return m_hashrate1h[WRAP(index)];
}

float History::getHashrate1dSample(int index)
{
    return m_hashrate1d[WRAP(index)];
}

double History::getCurrentHashrate10m()
{
    return m_avg10m.getGh();
}

double History::getCurrentHashrate1h()
{
    return m_avg10m.getGh();
}

double History::getCurrentHashrate1d()
{
    return m_avg10m.getGh();
}

uint32_t History::getShareSample(int index)
{
    return m_shares[WRAP(index)];
}

void History::lock()
{
    pthread_mutex_lock(&m_mutex);
}

void History::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

bool History::isAvailable()
{
    return BaseHistory::isAvailable() && m_shares && m_hashrate10m && m_hashrate1h && m_hashrate1d;
}

History::History() : m_avg10m(600llu * 1000llu), m_avg1h(3600llu * 1000llu), m_avg1d(86400llu * 1000llu)
{
    // NOP
}

bool History::init(int num_asics)
{
    if (!BaseHistory::init(num_asics)) {
        return false;
    }
    m_shares = (uint32_t *) heap_caps_malloc(HISTORY_MAX_SAMPLES * sizeof(uint32_t), MALLOC_CAP_SPIRAM);
    m_hashrate10m = (float *) heap_caps_malloc(HISTORY_MAX_SAMPLES * sizeof(float), MALLOC_CAP_SPIRAM);
    m_hashrate1h = (float *) heap_caps_malloc(HISTORY_MAX_SAMPLES * sizeof(float), MALLOC_CAP_SPIRAM);
    m_hashrate1d = (float *) heap_caps_malloc(HISTORY_MAX_SAMPLES * sizeof(float), MALLOC_CAP_SPIRAM);

    m_distribution.init(num_asics);

    return isAvailable();
}

void History::pushShare(uint32_t diff, uint64_t timestamp, int asic_nr)
{
    if (!isAvailable()) {
        ESP_LOGW(TAG, "PSRAM not initialized");
        return;
    }

    lock();
    m_shares[WRAP(m_numSamples)] = diff;
    History::push(timestamp);
    m_numSamples++;

    update(m_avg10m);
    update(m_avg1h);
    update(m_avg1d);

    m_hashrate10m[WRAP(m_numSamples - 1)] = m_avg10m.getGh();
    m_hashrate1h[WRAP(m_numSamples - 1)] = m_avg1h.getGh();
    m_hashrate1d[WRAP(m_numSamples - 1)] = m_avg1d.getGh();

    m_distribution.addShare(asic_nr);

    unlock();

    char preliminary_10m = (m_avg10m.isPreliminary()) ? '*' : ' ';
    char preliminary_1h = (m_avg1h.isPreliminary()) ? '*' : ' ';
    char preliminary_1d = (m_avg1d.isPreliminary()) ? '*' : ' ';

    ESP_LOGI(TAG, "hashrate: 10m:%.3fGH%c 1h:%.3fGH%c 1d:%.3fGH%c", m_avg10m.getGh(), preliminary_10m, m_avg1h.getGh(),
             preliminary_1h, m_avg1d.getGh(), preliminary_1d);

    m_distribution.toLog();
}


// move avg window and track and adjust the total sum of all shares in the
// desired time window. Calculates GH.
// calculates incrementally without "scanning" the entire time span
void History::update(HistoryHashRate &hist)
{
    // Catch up with the latest sample and update diffsum
    uint64_t lastTimestamp = 0;
    while (lastTimestamp = getTimestampSample(hist.m_lastSample), hist.m_lastSample + 1 < getNumSamples()) {
        hist.m_lastSample++;
        hist.m_diffSum += (uint64_t) getShareSample(hist.m_lastSample);
    }

    // adjust the window on the older side
    // we move the lower window bound until the next sample would be out of
    // the desired timespan.
    while (hist.m_firstSample + 1 < hist.m_lastSample && lastTimestamp - getTimestampSample(hist.m_firstSample + 1) >= hist.m_timespan) {
        hist.m_diffSum -= (uint64_t) getShareSample(hist.m_firstSample);
        hist.m_firstSample++;
    }

    uint64_t firstTimestamp = getTimestampSample(hist.m_firstSample);

    // Check for overflow in diffsum
    if (hist.m_diffSum >> 63ull) {
        ESP_LOGE(TAG, "Error in hashrate calculation: diffsum overflowed");
        return;
    }

    // Prevent division by zero
    if (lastTimestamp == firstTimestamp) {
        ESP_LOGW(TAG, "Timestamps are equal; cannot compute average.");
        return;
    }

    // Calculate the average hash rate
    uint64_t duration = (lastTimestamp - firstTimestamp);

    // preliminary means that it's not the real hashrate because
    // it's ramping up slowly
    hist.m_preliminary = duration < hist.m_timespan;

    // clamp duration to a minimum value of avg->timespan
    duration = (hist.m_timespan > duration) ? hist.m_timespan : duration;

    hist.m_avg = (double) (hist.m_diffSum << 32llu) / ((double) duration / 1.0e3);
    hist.m_avgGh = hist.m_avg / 1.0e9;
    hist.m_timestamp = lastTimestamp;
}
