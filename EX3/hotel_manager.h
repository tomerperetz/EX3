#pragma once


/*
====================================================================================================================
Description:
Caculate student grade main functions
====================================================================================================================
*/

// Includes --------------------------------------------------------------------------------------------------------
#pragma once
#include "hardCodedData.h"


// Defines  --------------------------------------------------------------------------------------------------------
#define MAX_GUEST_NAME_LEN 20
#define MAX_ROOM_NAME_LEN 20
#define MAX_NUM_OF_GUESTS 15
#define MAX_NUM_OF_ROOMS 5
#define MAX_LINE_LENGTH 50
#define GUEST_WAIT 0
#define GUEST_REGISTERED 1
#define GUEST_CHECK_OUT 2
#define GUEST_LEFT 3

// Structs ---------------------------------------------------------------------------------------------------------
typedef struct _Guest_struct
{
	char name[MAX_GUEST_NAME_LEN];
	int ID;
	int room_number;
	int status;
	int budget;
	int total_number_of_nights;
	int check_in_day;
} Guest_struct;

typedef struct _Room_struct
{
	char name[MAX_ROOM_NAME_LEN];
	int ID;
	int price_pp;
	int capacity;
	int availablity;
	int next_day_availablity;
	int waiting_guest_counter;
} Room_struct;

// Declerations ---------------------------------------------------------------------------------------------------

int runHotelWithThreads();

int createRoomMutex();

void printGuestStruct(Guest_struct guest_arr[MAX_NUM_OF_GUESTS]);

void printRoomStruct();

int readGuestFile(char dir_path[], Guest_struct guest_arr[MAX_NUM_OF_GUESTS]);

int readRoomFile(char dir_path[]);

int getRoomDataFromLine(char *line, char room_name[], int *price, int *capacity);
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id);

int readGuestFile(char dir_path[], Guest_struct guest_arr[MAX_NUM_OF_GUESTS]);

int getGuestDataFromLine(char *line, char guest_name[], int *budget);

/*
===================================================================================================================
											File handler - START
===================================================================================================================
*/

int writeMessegeBuffer(Guest_struct *p_guest, char *buffer, int max_size, char *mode);

int fileExist(char *filename);

int writeToLog(char *output_dir_path, char *content);

int logManager(Guest_struct *p_guest, char *output_dir_path, char *mode);


/*
===================================================================================================================
											Check customer in - START
===================================================================================================================
*/

int getRoomForGuest(Guest_struct *p_guest);

int registerRoomParllel(Guest_struct *p_guest);

int isRoomAvailableWrapper(Guest_struct *p_guest);

int isRoomAvaiable(Room_struct p_room);

int updateBudget(Guest_struct *p_guest);

int updateRoomAvaiabilty(Guest_struct *p_guest);

int updateCustomerStatus(Guest_struct *p_guest, int status);

int guestCheckInProcedure(Guest_struct *p_guest);

int registerRoom(Guest_struct *p_guest);

int checkIn(Guest_struct *p_guest);


/*
===================================================================================================================
											One more night - START
===================================================================================================================
*/

int CheckBudget(Guest_struct *p_guest);

int oneMoreNight(Guest_struct *p_guest);


/*
===================================================================================================================
											Checkout - START
===================================================================================================================
*/

int checkOut(Guest_struct *p_guest);

/*
===================================================================================================================
											EndOfDay - START
===================================================================================================================
*/

int endOfDay();

/*
===================================================================================================================
											Manager demo - no threads - START
===================================================================================================================
*/

int hotelManager(LPVOID p_guest);