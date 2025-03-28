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

#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
#pragma GCC diagnostic error "-Wmissing-prototypes"

HistoryAverage::HistoryAverage(uint64_t timespan)
{
    m_timespan = timespan;
}

