/*
====================================================================================================================
Authors:
	- Segev Elmalem, ID: 203149000
	- Tomer Peretz, ID: 305002206
Project: EX3
Input: folder path that contatins the following:
			- rooms.txt - a list of room names, prices and capacity
			- names.txt - a list of names and budget 
Outputs: <user_path>/roomsLog.txt
Description:
Register people to rooms according to budget. print out a log of when people checked in and when did they checked out.
====================================================================================================================
*/

// Includes --------------------------------------------------------------------
#include "hardCodedData.h"
Guest_struct guest_arr[MAX_NUM_OF_GUESTS];

// Functions --------------------------------------------------------------------

int main(int argc, char *argv[])
{
	extern Guest_struct guest_arr[MAX_NUM_OF_GUESTS];
	extern int num_of_guests;

	int idx = 0;
	// Checks whether the given arguments are valid  
	if (ensureArgs(argc, EXPECTED_ARGC, argv) != TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return TRUE;
	}
	if (readGuestFile(argv[1], guest_arr) != TRUE) {
		return TRUE;
	}
	//printGuestStruct(guest_arr);
	if (readRoomFile(argv[1]) != TRUE) {
		return TRUE;
	}
	//printf("\n\n=========================BEFORE===============================\n\n");
	//printRoomStruct();
	//printGuestStruct(guest_arr);
	//printf("\n\n=========================BEFORE===============================\n\n");
	runHotelWithThreads(&guest_arr);
	printf("\n\n=========================AFTER===============================\n\n");
	printRoomStruct();
	printGuestStruct(guest_arr);
	printf("\n\n=========================AFTER===============================\n\n");

	printf("Program ended successfully!\n");
	return FALSE;
}