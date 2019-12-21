/*
====================================================================================================================
description:
operation system functions: proccesses and threads.
====================================================================================================================
*/

// includes ---------------------------------------------------------------------------
#include "lib_oshandler.h"


//// Thread Function Definitions --------------------------------------------------------



int closeHandles(const HANDLE *p_thread_handles, int size) {
	/*
	Description: create thread with arg wrapper
	parameters:
			- HANDLE *p_thread_handles - array of handles
			- int size - size of handles araay
	Return: true if all the handles closed safly 
	*/
	int idx = 0, retVal = TRUE;
	for (idx = 0; idx < size; idx++) {
		if (p_thread_handles[idx] != NULL) {
			if (CloseHandle(p_thread_handles[idx]) == FALSE) {
				raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
				printf("Details: Error when closing thread\n");
				retVal = FALSE;
			}
		}
	}
	return retVal;
}


static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	/*
	Description: create thread with arg wrapper
	parameters:
			- LPTHREAD_START_ROUTINE p_start_routine
			- LPVOID p_thread_parameters
			- LPDWORD p_thread_id
	Return: thread_handle if succeded, ERR o.w
	*/

	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("Details: Error when creating a thread");
		printf("\tReceived null pointer");
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("Details: Error when creating a thread");
		printf("\tReceived null pointer");
		return NULL;
	}

	thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		p_thread_parameters, /*  argument to thread function */
		0,                   /*  use default creation flags */
		p_thread_id);        /*  returns the thread identifier */

	return thread_handle;
}
