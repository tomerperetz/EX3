/*
====================================================================================================================
Authors:
	- Segev Elmalem, ID: 203149000
	- Tomer Peretz, ID: 305002206
Project: TestManager
Input: students grades directory path
Outputs: <user_path>/final_grades.txt
Description:
Calculate grades for each student and prints summary to output file.
====================================================================================================================
*/

// Includes --------------------------------------------------------------------

#include "hardCodedData.h"


// Functions --------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Checks whether the given arguments are valid  
	if (ensureArgs(argc, EXPECTED_ARGC, argv) != TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return TRUE;
	}

	printf("Program ended successfully!\n");
	return FALSE;
}