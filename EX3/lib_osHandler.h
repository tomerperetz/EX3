/*
====================================================================================================================
Description:
Operation System functions: Proccesses and Threads.
====================================================================================================================
*/

#pragma once
#ifdef _MSC_VER
#endif

// Includes --------------------------------------------------------------------
#include "hardCodedData.h"

// Types -----------------------------------------------------------------------


// Declerations ------------------------------------------------------------------------

// Thread Functions
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id);
int closeHandles(const HANDLE *p_thread_handles, int size);