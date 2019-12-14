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


// Structs ---------------------------------------------------------------------------------------------------------
typedef struct _Guest_struct
{
	char name[MAX_GUEST_NAME_LEN];
	int ID;
	int room_number;
	int status;
	int budget;
	int total_number_of_nights;
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

