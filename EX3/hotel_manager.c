#include "hotel_manager.h"

// Global variables -------------------------------------------------------------------------------------->
Room_struct room_arr[MAX_NUM_OF_ROOMS];
int day_counter = 1;
int num_of_guests = 0;
int num_of_rooms = 0;


void printGuestStruct(Guest_struct guest_arr[MAX_NUM_OF_GUESTS]) {
	extern int num_of_guests;
	int idx = 0;
	for (idx = 0; idx < num_of_guests; idx++) {
		
		printf("*******************************\n");
		printf("| Guest Name:        | %s\n", guest_arr[idx].name);
		printf("*******************************\n");
		printf("| Guest Budget:      | %d\n", guest_arr[idx].budget);
		printf("-------------------------------\n");
		printf("| Guest ID:          | %d\n", guest_arr[idx].ID);
		printf("-------------------------------\n");
		printf("| Guest Room #:      | %d\n", guest_arr[idx].room_number);
		printf("-------------------------------\n");
		printf("| Guest Status       | %d\n", guest_arr[idx].status);
		printf("-------------------------------\n");
		printf("| Gueat Tot # nights:| %d\n", guest_arr[idx].total_number_of_nights);
		printf("-------------------------------\n");
	}
	printf("Total number of Guests: %d\n", num_of_guests);
}

void printRoomStruct() 
{
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	extern int num_of_rooms;
	int idx = 0;
	for (idx = 0; idx < num_of_rooms; idx++) {

		printf("***********************************************\n");
		printf("| Room Name:                    | %s\n", room_arr[idx].name);
		printf("***********************************************\n");
		printf("| Room Price:                   | %d\n", room_arr[idx].price_pp);
		printf("-----------------------------------------------\n");
		printf("| Room Capacity:                | %d\n", room_arr[idx].capacity);
		printf("-----------------------------------------------\n");
		printf("| Room ID:                      | %d\n", room_arr[idx].ID);
		printf("-----------------------------------------------\n");
		printf("| Room Availabilty #:           | %d\n", room_arr[idx].availablity);
		printf("-----------------------------------------------\n");
		printf("| Room next day Availabilty     | %d\n", room_arr[idx].next_day_availablity);
		printf("-----------------------------------------------\n");
		printf("| Room waiting list counter     | %d\n", room_arr[idx].waiting_guest_counter);
		printf("-----------------------------------------------\n");
	}
	printf("Total number of Rooms: %d\n", num_of_rooms);
}

int readRoomFile(char dir_path[]) 
{
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	extern int num_of_rooms;
	FILE *fp = NULL;
	char file_name[11] = "/rooms.txt", *file_path = NULL, room_name[MAX_ROOM_NAME_LEN], line[MAX_LINE_LENGTH];
	int capacity = 0, price = 0, ret_val = TRUE;

	if (strcatDynamic(dir_path, file_name, &file_path) == FALSE) {
		return ERR;
	}
	if (fopen_s(&fp, file_path, "r") != FALSE || fp == NULL) {
		printf("Can't open file: %s\n", file_path);
		str_safe_free(file_path);
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		return ERR;
	}
	str_safe_free(file_path);
	while (!feof(fp) && num_of_guests <= MAX_NUM_OF_GUESTS) {
		fgets(line, MAX_LINE_LENGTH, fp);
		if (getRoomDataFromLine(line, room_name, &price, &capacity) != TRUE) {
			ret_val = ERR;
			break;
		}
		strcpy_s(room_arr[num_of_rooms].name, MAX_ROOM_NAME_LEN, room_name);
		room_arr[num_of_rooms].price_pp = price;
		room_arr[num_of_rooms].capacity = capacity;
		room_arr[num_of_rooms].availablity = capacity;
		room_arr[num_of_rooms].next_day_availablity = 0;
		room_arr[num_of_rooms].ID = num_of_rooms;
		room_arr[num_of_rooms].waiting_guest_counter = 0;
		num_of_rooms++;
	}
	if (fclose(fp) != FALSE)
	{
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("closing file FAILED\nFile: %s\n", file_path);
		return ERR;
	}
	return ret_val;
}

int getRoomDataFromLine(char *line, char room_name[], int *price, int *capacity)
{
	char space = ' ', price_str[MAX_LINE_LENGTH], capacity_str[MAX_LINE_LENGTH];
	int line_len = 0, idx = 0, price_idx = 0;
	if (line == NULL) {
		raiseError(10, __FILE__, __func__, __LINE__, "Guest Name File has an invalid line");
		return ERR;
	}
	line_len = strlen(line);
	while ((idx < line_len) && line[idx] != space) {
		room_name[idx] = line[idx];
		idx++;
	}
	room_name[idx] = END_OF_STR;
	idx++;

	while ((idx < line_len) && (line[idx] > 47) && (line[idx] < 58) && line[idx] != space) {
		price_str[price_idx] = line[idx];
		idx++;
		price_idx++;
	}
	if (price_str == NULL) {
		raiseError(10, __FILE__, __func__, __LINE__, "Guest Name File has an invalid line");
		return ERR;
	}
	price_str[price_idx] = END_OF_STR;
	*price = atoi(price_str);
	idx++;
	price_idx = 0;

	while ((idx < line_len) && (line[idx] > 47) && (line[idx] < 58)) {
		capacity_str[price_idx] = line[idx];
		idx++;
		price_idx++;
	}
	if (capacity_str == NULL) {
		raiseError(10, __FILE__, __func__, __LINE__, "Guest Name File has an invalid line");
		return ERR;
	}
	capacity_str[price_idx] = END_OF_STR;
	*capacity = atoi(capacity_str);
	return TRUE;
}

int readGuestFile(char dir_path[], Guest_struct guest_arr[MAX_NUM_OF_GUESTS])
{
	extern int num_of_guests;
	int idx = 0, budget = 0, ret_val = TRUE;
	FILE *fp = NULL;
	char file_name[11] = "/names.txt", *file_path = NULL, guest_name[MAX_GUEST_NAME_LEN], line[MAX_LINE_LENGTH];

	if (strcatDynamic(dir_path, file_name, &file_path) == FALSE) {
		return ERR;
	}
	
	if (fopen_s(&fp, file_path, "r") != FALSE || fp == NULL) {
		printf("Can't open file: %s\n", file_path);
		str_safe_free(file_path);
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		return ERR;
	}
	str_safe_free(file_path);
	
	while (!feof(fp) && num_of_guests <= MAX_NUM_OF_GUESTS) {
		fgets(line, MAX_LINE_LENGTH, fp);
		if (getGuestDataFromLine(line, guest_name, &budget) != TRUE) {
			ret_val = ERR;
			break;
		}
		
		guest_arr[num_of_guests].budget = budget;
		strcpy_s(guest_arr[num_of_guests].name, MAX_GUEST_NAME_LEN, guest_name);
		guest_arr[num_of_guests].ID = num_of_guests;
		guest_arr[num_of_guests].room_number = -1;
		guest_arr[num_of_guests].status = GUEST_WAIT;
		guest_arr[num_of_guests].total_number_of_nights = 0;
		guest_arr[num_of_guests].check_in_day = -1;

		num_of_guests += 1;
	}
	if (fclose(fp) != FALSE)
	{
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("closing file FAILED\nFile: %s\n", file_path);
		return ERR;
	}
	return ret_val;
}

int getGuestDataFromLine(char *line, char guest_name[], int *budget)
{
	char space = ' ', budget_str[MAX_LINE_LENGTH];
	int line_len = 0, idx = 0, budget_idx = 0;
	if (line == NULL) {
		raiseError(10, __FILE__, __func__, __LINE__, "Guest Name File has an invalid line");
		return ERR;
	}

	line_len = strlen(line);
	while ((idx < line_len) && line[idx] != space) {
		guest_name[idx] = line[idx];
		idx++;
	}
	guest_name[idx] = END_OF_STR;
	idx++;

	while ((idx < line_len) && (line[idx] > 47) && (line[idx] < 58)) {
		budget_str[budget_idx] = line[idx];
		idx++;
		budget_idx++;
	}
	if (budget_str == NULL) {
		raiseError(10, __FILE__, __func__, __LINE__, "Guest Name File has an invalid line");
		return ERR;
	}
	budget_str[budget_idx] = END_OF_STR;
	*budget = atoi(budget_str);

	return TRUE;
}


// Functions --------------------------------------------------------------------------------------------->
/*
===================================================================================================================
											Check customer in - START
===================================================================================================================
*/

int getRoomForGuest(Guest_struct *p_guest)
{
	/* Init room type for customer according to his budget */
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	extern int num_of_rooms;
	extern int day_counter;

	for (int i = 0; i < num_of_rooms; i++)
	{
		if ((p_guest->budget % room_arr[i].price_pp) == 0)
		{
			p_guest->room_number = i;
			p_guest->total_number_of_nights = (p_guest->budget / room_arr[i].price_pp);
			p_guest->check_in_day = day_counter;
			return TRUE;
		}
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
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	p_guest->budget = p_guest->budget - room_arr[p_guest->room_number].price_pp;
	return TRUE;
}

int updateRoomAvaiabilty(Guest_struct *p_guest)
{
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	
	/* Update for tonight */
	room_arr[p_guest->room_number].availablity--;
	if (p_guest->budget==0)
		room_arr[p_guest->room_number].next_day_availablity++;
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
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];

	/* check if room currently avaiable */
	if (isRoomAvaiable(room_arr[p_guest->room_number]))
		/* Check customer in */
		if (guestCheckInProcedure(p_guest)!=TRUE)
		{
			printf("Couldn't check customer in.\n");
			printf("Name: %s\nRoom: %s\nPrice: %d\nBudget: %d\n", p_guest->name, room_arr[p_guest->room_number].name, room_arr[p_guest->room_number].price_pp, p_guest->budget);
			raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
			return ERR;
		}
	return TRUE;
}

int CheckIn(Guest_struct *p_guest)
{
	if (p_guest->room_number == ERR)
	{
		/* search room for guest */
		if (getRoomForGuest(p_guest) != TRUE)
		{
			printf("Couldn't allocate room to this guest! please update his budget.\n");
			printf("Name: %s\nRoom: %s\nPrice: %d\nBudget: %d", p_guest->name, room_arr[p_guest->room_number].name, room_arr[p_guest->room_number].price_pp, p_guest->budget);
			raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
			return ERR;
		}
	}
	

	/* try to register room to guest */
	if (registerRoom(p_guest) != TRUE)
	{
		printf("some error in register room\n");
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

int demoRoomsStruct()
{
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	Guest_struct guest_1, guest_2;
	printf("working\n");
	for (int i = 0; i < MAX_NUM_OF_ROOMS; i++)
	{
		room_arr[i].availablity = i + 1;
		room_arr[i].capacity = i + 1;
		room_arr[i].ID = i;
		strcpy_s(room_arr[i].name,8,"my_room");
		room_arr[i].next_day_availablity = 0;
		room_arr[i].price_pp = 30;
		room_arr[i].waiting_guest_counter = 0;
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