#ifndef ASIC_TASK_H_
#define ASIC_TASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "mining.h"

#define MAX_ASIC_JOBS 128

typedef struct
{
    // ASIC may not return the nonce in the same order as the jobs were sent
    // it also may return a previous nonce under some circumstances
    // so we keep a list of jobs indexed by the job id
    bm_job *active_jobs[MAX_ASIC_JOBS];
} AsicTaskModule;



#endif
