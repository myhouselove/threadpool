#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct sys_cpu_info
{
	char name[6];
	unsigned long user;
	unsigned long nic;
	unsigned long system;
	unsigned long idle;
	
}SysCPUInfo;

SysCPUInfo* 
_GetHostCPUInfo()
{
	SysCPUInfo *cpuinfo = (SysCPUInfo *)malloc(sizeof(SysCPUInfo));
	if (cpuinfo == NULL)
	  printf("_GetCPUInfo: malloc struct SysCPUInfo error");

	FILE 	*fd;
	char	buff[256];
	memset(buff, '\0', 256);

	fd = fopen("/proc/stat", "r");
	fgets(buff, sizeof(buff), fd);

	sscanf(buff, "%s %lu %lu %lu %lu", cpuinfo->name, &cpuinfo->user, &cpuinfo->nic, 
				&cpuinfo->system, &cpuinfo->idle);
	fclose(fd);
	return cpuinfo;
}

float
_CalculateHostCPURate(SysCPUInfo *first, SysCPUInfo *second)
{
	unsigned long	old_CPU_Time, new_CPU_Time;
	unsigned long	usr_Time_Diff, sys_Time_Diff, nic_Time_Diff;
	float 			cpu_use = 0.0;

	old_CPU_Time = (unsigned long)(first->user + first->nic + first->system + first->idle);
	new_CPU_Time = (unsigned long)(second->user + second->nic + second->system + second->idle);

	usr_Time_Diff = (unsigned long)(second->user - first->user);
	sys_Time_Diff = (unsigned long)(second->system - first->system);
	nic_Time_Diff = (unsigned long)(second->nic -first->nic);

	if ((new_CPU_Time - old_CPU_Time) != 0)
	  cpu_use = (float)100*(usr_Time_Diff + sys_Time_Diff + nic_Time_Diff)/(new_CPU_Time - old_CPU_Time);
	else
	  cpu_use = 0.0;
	return cpu_use;
}

float
GetHostCPURate()
{
	float cpu_rate;
	SysCPUInfo *first, *second;
	first = _GetHostCPUInfo();
	sleep(5);
	second = _GetHostCPUInfo();

	cpu_rate = _CalculateHostCPURate(first, second);

	/* clean auxiliary memory */
	free(first);
	free(second);
	first = second = NULL;

	return cpu_rate;
}

