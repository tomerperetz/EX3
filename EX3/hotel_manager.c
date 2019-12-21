#include "hotel_manager.h"

// Global variables -------------------------------------------------------------------------------------->
Room_struct p_room_arr[MAX_NUM_OF_ROOMS];
int day_counter = 1;

// Functions --------------------------------------------------------------------------------------------->
/*
===================================================================================================================
											Check customer in - START
===================================================================================================================
*/
int getRoomForGuest(Guest_struct *p_guest)
{
	/* Init room type for customer according to his budget */
	extern Room_struct p_room_arr[MAX_NUM_OF_ROOMS];

	for (int i = 0; i < MAX_NUM_OF_ROOMS; i++)
	{
		if ((p_guest->budget % p_room_arr[i].price_pp) == 0)
		{
			p_guest->room_number = i;
			printf("customer room is: %s, he has the budget of: %d\n", p_guest->name, p_guest->budget);
			return TRUE;
		}
		printf("customer didn't get room: %s, he has the budget of: %d\n", p_guest->name, p_guest->budget);
	}
	return FALSE;
}

int isRoomAvaiable(Room_struct p_room)
{
	/* Check if room is currently avaiable */
	if (p_room.availablity > 0)
		return TRUE;
	return FALSE;
}

int updateBudget(Guest_struct *p_guest)
{
	/* decrease customer budget */
	extern Room_struct p_room_arr[MAX_NUM_OF_ROOMS];
	p_guest->budget = p_guest->budget - p_room_arr[p_guest->room_number].price_pp;
	return TRUE;
}

int updateRoomAvaiabilty(Guest_struct *p_guest)
{
	extern Room_struct p_room_arr[MAX_NUM_OF_ROOMS];
	
	/* Update for tonight */
	p_room_arr[p_guest->room_number].availablity--;
	if (p_guest->budget==0)
		p_room_arr[p_guest->room_number].next_day_availablity++;
	return TRUE;
}

int updateCustomerStatus(Guest_struct *p_guest, int status)
{
	p_guest->status = status;
	return TRUE;
}

int guestCheckInProcedure(Guest_struct *p_guest)
{
	/* Update: room avaiablity, room avaiablity next day, customer budget, customer status */
	/* update budget */
	updateBudget(p_guest);
	updateCustomerStatus(p_guest, GUEST_REGISTERED);
	updateRoomAvaiabilty(p_guest);
	return TRUE;
}

int registerRoom(Guest_struct *p_guest)
{ 
	/* Need to wrap with mutex */
	extern Room_struct p_room_arr[MAX_NUM_OF_ROOMS];

	/* check if room currently avaiable */
	if (isRoomAvaiable(p_room_arr[p_guest->room_number]))
		/* Check customer in */
		if (guestCheckInProcedure(p_guest)!=TRUE)
		{
			printf("Couldn't check customer in.\n");
			printf("Name: %s\nRoom: %s\nPrice: %d\nBudget: %d\n", p_guest->name, p_room_arr[p_guest->room_number].name, p_room_arr[p_guest->room_number].price_pp, p_guest->budget);
			raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
			return ERR;
		}
	return TRUE;
}

int CheckIn(Guest_struct *p_guest)
{
	/* search room for guest */
	if (getRoomForGuest(p_guest) != TRUE)
	{
		printf("Couldn't allocate room to this guest! please update his budget.\n");
		printf("Name: %s\nRoom: %s\nPrice: %d\nBudget: %d", p_guest->name, p_room_arr[p_guest->room_number].name, p_room_arr[p_guest->room_number].price_pp, p_guest->budget);
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
		return ERR;
	}	
	/* try to register room to guest */
	if (registerRoom(p_guest) != TRUE)
	{
		/* couldn't check customer in */
	}


	return TRUE;
}

/*
===================================================================================================================
											Check customer in - END
===================================================================================================================
*/


/*
===================================================================================================================
											One more night - START
===================================================================================================================
*/



/*
===================================================================================================================
											One more night in - END
===================================================================================================================
*/





/* DEMO ----------------------------------------------------------------------------------------------------------------------------> */
void printRooms()
{
	extern Room_struct p_room_arr[MAX_NUM_OF_ROOMS];
	
	for (int i = 0; i < 1; i++)
	{
		printf("Room name: %s\n", p_room_arr[i].name);
		printf("Room ID: %d\n", p_room_arr[i].ID);
		printf("Room price_pp: %d\n", p_room_arr[i].price_pp);
		printf("Room capacity: %d\n", p_room_arr[i].capacity);
		printf("Room availablity: %d\n", p_room_arr[i].availablity);
		printf("Room next_day_availablity: %d\n", p_room_arr[i].next_day_availablity);
		printf("Room waiting_guest_counter: %d\n", p_room_arr[i].waiting_guest_counter);
	}
	
}

void printGuest(Guest_struct *p_guest)
{
	printf("guest name: %s\n", p_guest->name);
	printf("guest ID: %d\n", p_guest->ID);
	printf("guest budget: %d\n", p_guest->budget);
	printf("guest room_number: %d\n", p_guest->room_number);
	printf("guest status: %d\n", p_guest->status);
	printf("guest total_number_of_nights: %d\n", p_guest->total_number_of_nights);

}


int demoRoomsStruct()
{
	extern Room_struct p_room_arr[MAX_NUM_OF_ROOMS];
	Guest_struct guest_1, guest_2;
	printf("working\n");
	for (int i = 0; i < MAX_NUM_OF_ROOMS; i++)
	{
		p_room_arr[i].availablity = i + 1;
		p_room_arr[i].capacity = i + 1;
		p_room_arr[i].ID = i;
		strcpy_s(p_room_arr[i].name,8,"my_room");
		p_room_arr[i].next_day_availablity = 0;
		p_room_arr[i].price_pp = 30;
		p_room_arr[i].waiting_guest_counter = 0;
	}

	guest_1.budget = 90;
	guest_1.ID = 1;
	guest_1.room_number = -1;
	guest_1.status = GUEST_WAIT;
	guest_1.total_number_of_nights = 0;
	strcpy_s(guest_1.name, 8, "tomer");

	guest_2.budget = 95;
	guest_2.ID = 2;
	guest_2.room_number = -2;
	guest_2.status = GUEST_WAIT;
	guest_2.total_number_of_nights = 0;
	strcpy_s(guest_2.name, 8, "segev");

	return TRUE;
}