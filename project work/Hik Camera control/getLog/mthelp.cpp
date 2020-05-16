/*
 * mthelp.cpp - multithread helper implementation
 *
 * Authors: Jia Wang
 */

// system headers
#ifdef _WIN32
// Win32 system
#include <windows.h>
#include <process.h>
#else // !defined(_WIN32)
// Unix like systems
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#endif // _WIN32

#include "mthelp.h"

namespace os
{

#ifdef _WIN32
// Win32 System

bool thread_set_affinity(int processor)
{
	if (processor < 0)
		return false;

	DWORD_PTR pm, sm;

	if (GetProcessAffinityMask(GetCurrentProcess(), &pm, &sm) == 0)
		return false;

	DWORD_PTR tm = (1ULL << processor)&pm;

	if (tm == 0) tm = pm;

	tm = SetThreadAffinityMask(GetCurrentThread(), tm);

	return tm != 0;
}

#else // !defined(_WIN32)
// Unix like systems

bool thread_set_affinity(int processor)
{
	if (processor < 0)
		return false;

	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(processor, &cpuset);

	int ret = pthread_setaffinity_np(pthread_self(),
		sizeof(cpu_set_t), &cpuset);

	return ret == 0;
}

#endif // _WIN32

} // namespace os
