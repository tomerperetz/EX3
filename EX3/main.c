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
char **g_argv;


// Functions --------------------------------------------------------------------
int checkWaitList(Guest_struct **guest_arr)
{
	extern int num_of_guests;
	int wait_list = 0;
	for (int idx = 0; idx < num_of_guests; idx++)
	{
		if (guest_arr[idx]->status == GUEST_WAIT)
			wait_list += 1;
	}
	return wait_list;
}

int main(int argc, char *argv[])
{
	extern Guest_struct guest_arr[MAX_NUM_OF_GUESTS];
	extern int num_of_guests;
	int wait_list = 0;
	int idx = 0;
	extern char **g_argv;
	g_argv = argv;

	// Checks whether the given arguments are valid  
	if (ensureArgs(argc, EXPECTED_ARGC, argv) != TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return TRUE;
	}
	if (readGuestFile(argv[1], guest_arr) != TRUE) {
		return TRUE;
	}
	// PrintGuestStruct(guest_arr);
	if (readRoomFile(argv[1]) != TRUE) {
		return TRUE;
	}

	runHotelWithThreads(&guest_arr);
	printf("\n\n=========================AFTER===============================\n\n");
	printRoomStruct();
	printGuestStruct(guest_arr);
	printf("\n\n=========================AFTER===============================\n\n");

	printf("Program ended successfully!\n");

	return 0;
}