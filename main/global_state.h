#pragma once

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#include "stratum_api.h"

#include "asic.h"
#include "bm1368.h"
#include "tasks/asic_jobs.h"
#include "tasks/power_management_task.h"
#include "tasks/stratum_task.h"
#include "tasks/apis_task.h"
#include "nvs_config.h"

#include "boards/nerdqaxeplus.h"
#include "system.h"

extern System SYSTEM_MODULE;
extern PowerManagementTask POWER_MANAGEMENT_MODULE;
extern StratumManager STRATUM_MANAGER;
extern APIsFetcher APIs_FETCHER;
extern Config CONFIG;
extern AsicJobs asicJobs;

