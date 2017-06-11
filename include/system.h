#ifndef __SYSTEM_H_INCLUDED__
#define __SYSTEM_H_INCLUDED__

float GetHostCPURate();


typedef void (*TaskFuncCallback)(void *arg);

typedef enum{
TASK_WAIT,
TASK_DOING,
TASK_DONE,
}TASK_STAUTUS_T;

typedef struct TASK
{
	TASK_STAUTUS_T sTask;
	TaskFuncCallback cTaskFunc;
	

}TASK_QUEUE_T;
#define MAX_TASK_NUM 50
#define NO_TASK_TIMEOUT 30


typedef enum API_RETURN_TYPE
{
	API_SUCCESS, //0
	API_FAIL
}API_RETURN_TYPE_T;

#define RET_OK 0





#endif
