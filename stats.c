#include "./stats.h"

#include <sys/types.h>
#include <sys/sysinfo.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const int TO_PER_CENT = 100;
static const char CPU_FILE_PATH[] = "/proc/stat";

static unsigned long long last_cpu_usage_total_user = 0;
static unsigned long long last_cpu_usage_total_user_low = 0;
static unsigned long long last_cpu_usage_total_system = 0;
static unsigned long long last_cpu_usage_total_idle = 0;

void init_statistics_retriever(void)
{
	FILE *cpu_process_information = fopen(CPU_FILE_PATH, "r");
	fscanf
	(
		cpu_process_information,
		"cpu %llu %llu %llu %llu",
		&last_cpu_usage_total_user, &last_cpu_usage_total_user_low, &last_cpu_usage_total_system, &last_cpu_usage_total_idle
	);
	fclose(cpu_process_information);
}

double get_cpu_usage(void)
{
	FILE *cpu_process_information = fopen(CPU_FILE_PATH, "r");
	unsigned long long cpu_usage_total_user = 0;
	unsigned long long cpu_usage_total_user_low = 0;
	unsigned long long cpu_usage_total_system = 0;
	unsigned long long cpu_usage_total_idle = 0;

	fscanf
	(
		cpu_process_information,
		"cpu %llu %llu %llu %llu",
		&cpu_usage_total_user, &cpu_usage_total_user_low, &cpu_usage_total_system, &cpu_usage_total_idle
	);
	fclose(cpu_process_information);

	double per_cent = 0;
	unsigned long long total_cpu_usage = 0;
	if
	(
		(cpu_usage_total_user < last_cpu_usage_total_user)
		|| (cpu_usage_total_user_low < last_cpu_usage_total_user_low)
		|| (cpu_usage_total_system < last_cpu_usage_total_system)
		|| (cpu_usage_total_idle < last_cpu_usage_total_idle)
	)
	{
		per_cent = (-1);
	}
	else
	{
		total_cpu_usage =
		(
			(cpu_usage_total_user - last_cpu_usage_total_user)
			+ (cpu_usage_total_user_low - last_cpu_usage_total_user_low)
			+ (cpu_usage_total_system - last_cpu_usage_total_system)
		);
		per_cent = ((double) total_cpu_usage);
		total_cpu_usage += (cpu_usage_total_idle - last_cpu_usage_total_idle);
		per_cent /= ((double) total_cpu_usage);
		per_cent *= TO_PER_CENT;
	}

	last_cpu_usage_total_user = cpu_usage_total_user;
	last_cpu_usage_total_user_low = cpu_usage_total_user_low;
	last_cpu_usage_total_system = cpu_usage_total_system;
	last_cpu_usage_total_idle = cpu_usage_total_idle;

	return per_cent;
}

double get_memory_usage(void)
{
	struct sysinfo memory_info;
	sysinfo(&memory_info);

	unsigned long total_physical_memory = memory_info.totalram;
	total_physical_memory *= memory_info.mem_unit;

	unsigned long physical_memory_used = (memory_info.totalram - memory_info.freeram);
	physical_memory_used *= memory_info.mem_unit;

	return ((((double) physical_memory_used) / ((double) total_physical_memory)) * TO_PER_CENT);
}
