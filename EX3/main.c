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
	Guest_struct p_guest_arr[MAX_NUM_OF_GUESTS];
	extern int num_of_guests;
	int idx = 0;
	// Checks whether the given arguments are valid  
	if (ensureArgs(argc, EXPECTED_ARGC, argv) != TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return TRUE;
	}
	if (readGuestFile(argv[1], p_guest_arr) != TRUE) {
		return TRUE;
	}
	//printGuestStruct(p_guest_arr);
	if (readRoomFile(argv[1]) != TRUE) {
		return TRUE;
	}
	printRoomStruct();
	
	printf("Program ended successfully!\n");
	return FALSE;
}