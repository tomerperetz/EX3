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

// Structs ---------------------------------------------------------------------------------------------------------

typedef struct _guest_struct
{
	char *name[MAX_GUEST_NAME_LEN];
	int ID;
	int room_number;
	int status;
	int budget;
} guest_struct;

typedef struct _hotel_room_struct
{
	char *name[MAX_GUEST_NAME_LEN];
	int ID;
	int price_pp;
	int capacity;
	int availablity;
	int next_day_availablity;
	int waiting_guest_counter;
} hotel_room_struct;

// Declerations ---------------------------------------------------------------------------------------------------

