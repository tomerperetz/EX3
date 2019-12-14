/*
====================================================================================================================
Description:
this lib is a parser for arguments recieved from user.
====================================================================================================================
*/

#pragma once
#ifdef _MSC_VER
#endif

#include "hardCodedData.h"


static const int EXPECTED_ARGC = 2;
static const char HELPER[1000] = \
"===========================================================================================\n"\
"This Program will caluculate the result of nested equation with the following form: '((X+Y)*Z)'\n"\
"Inputs: equation - nested equation. you may use only '+' or '*' operators. every sub equation must be within Parenthesis\n"\
"Output: results will be printed to an output file by the order of calulation. \n"\
"===========================================================================================\n";

int isDirectory(const char *path);
int ensureArgs(int argc, int expected_argc, char *argv[]);
int isArgsValid(int argc, char *argv[]);
void callHelper();

