/*
====================================================================================================================
Description:
Hard coded data: includes and defines
====================================================================================================================
*/

// Pragma ------------------------------------------------------>
#pragma once
#ifdef _MSC_VER
#endif

// Includes --------------------------------------------------->
#include <stdio.h>	
#include <string.h>
#include <windows.h>
#include "argparser.h"
#include "lib_errorHandler.h"
#include "lib_osHandler.h"
#include "lib_str_func.h"
#include <sys/stat.h>

// Consts ----------------------------------------------------->
static const int ERR = -1;
static const int EQUAL = 0;
static const char END_OF_STR = '\0';

//defines ----------------------------------------------------->
#define MAX_ROOM_NAME_LEN 20
#define MAX_GUEST_NAME_LEN 20
#define MAX_NUM_OF_ROOMS 5
#define MAX_NUM_OF_GUESTS 15
#define MAX_LINE_LENGTH 50
