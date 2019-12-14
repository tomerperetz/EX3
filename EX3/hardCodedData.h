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
#include <sys/stat.h>

#include "argparser.h"
#include "lib_errorHandler.h"
#include "lib_osHandler.h"
#include "lib_str_func.h"
#include "hotel_manager.h"

// Consts ----------------------------------------------------->
static const int ERR = -1;
static const int EQUAL = 0;
static const char END_OF_STR = '\0';

//defines ----------------------------------------------------->
#define MAX_NUM_OF_ROOMS 5
#define MAX_NUM_OF_GUESTS 15
#define MAX_LINE_LENGTH 50
#define GUEST_WAIT 0
#define GUEST_REGISTERED 1
#define GUEST_CHECK_OUT 2
#define GUEST_LEFT 3
