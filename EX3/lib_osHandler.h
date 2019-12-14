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

typedef enum
{
	STUDENT_GRADE_TREAD__CODE_SUCCESS,
	STUDENT_GRADE_TREAD__CODE_NULL_PTR,
} STUDENT_GRADE_TREAD__return_code_t;

/*
* A type for thread parameters
*/
typedef struct
{
	int *cur_grade;
	char **cur_file;
} STUDENT_GRADE_TREAD_params_t;


// Declerations ------------------------------------------------------------------------

// Process functions
int CreateProcessSimpleMain(char *command, char *id);
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);

// Thread Functions
void freeThreadParamMem(STUDENT_GRADE_TREAD_params_t **p_thread_params_arr);
int mainCreateReadGradesThreadSimple(char **files_list, int *grades_list);
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id);
int closeHandles(const HANDLE *p_thread_handles, int size);