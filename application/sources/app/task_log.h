#ifndef __TASK_LOG_H__
#define __TASK_LOG_H__

#define BUFFER_LOG_SIZE		(20)

#include "log.h"


extern ring_log_buffer_t ring_buffer_log;
extern uint8_t buffer_log[BUFFER_LOG_SIZE];





#endif //__TASK_LOG_H__
