#include "hotel_manager.h"

// global variables -------------------------------------------------------------------------------------->
Room_struct room_arr[MAX_NUM_OF_ROOMS];
int day_counter = 1;
int num_of_guests = 0;
int num_of_rooms = 0;


void printGuestStruct(Guest_struct p_guest_arr[MAX_NUM_OF_GUESTS]) {
	extern int num_of_guests;
	int idx = 0;
	for (idx = 0; idx < num_of_guests; idx++) {
		
		printf("*******************************\n");
		printf("| Guest Name:        | %s\n", p_guest_arr[idx].name);
		printf("*******************************\n");
		printf("| Guest Budget:      | %d\n", p_guest_arr[idx].budget);
		printf("-------------------------------\n");
		printf("| Guest ID:          | %d\n", p_guest_arr[idx].ID);
		printf("-------------------------------\n");
		printf("| Guest Room #:      | %d\n", p_guest_arr[idx].room_number);
		printf("-------------------------------\n");
		printf("| Guest Status       | %d\n", p_guest_arr[idx].status);
		printf("-------------------------------\n");
		printf("| Gueat Tot # nights:| %d\n", p_guest_arr[idx].total_number_of_nights);
		printf("-------------------------------\n");
	}
	printf("Total number of Guests: %d\n", num_of_guests);
}

void printRoomStruct() {
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	extern int num_of_rooms;
	int idx = 0;
	for (idx = 0; idx < num_of_rooms; idx++) {

		printf("*******************************\n");
		printf("| Room Name:        | %s\n", room_arr[idx].name);
		printf("*******************************\n");
		printf("| Room Price:      | %d\n", room_arr[idx].price_pp);
		printf("-------------------------------\n");
		printf("| Room Capacity:      | %d\n", room_arr[idx].capacity);
		printf("-------------------------------\n");
		printf("| Room ID:          | %d\n", room_arr[idx].ID);
		printf("-------------------------------\n");
		printf("| Room Room #:      | %d\n", room_arr[idx].availablity);
		printf("-------------------------------\n");
		printf("| Room Status       | %d\n", room_arr[idx].next_day_availablity);
		printf("-------------------------------\n");
		printf("| Room Status       | %d\n", room_arr[idx].waiting_guest_counter);
		printf("-------------------------------\n");
	}
	printf("Total number of Rooms: %d\n", num_of_rooms);
}
int readRoomFile(char dir_path[]) {
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
		room_arr[num_of_rooms].next_day_availablity = capacity;
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

int getRoomDataFromLine(char *line, char room_name[], int *price, int *capacity) {
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

int readGuestFile(char dir_path[], Guest_struct p_guest_arr[MAX_NUM_OF_GUESTS])
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
		
		p_guest_arr[num_of_guests].budget = budget;
		strcpy_s(p_guest_arr[num_of_guests].name, MAX_GUEST_NAME_LEN, guest_name);
		p_guest_arr[num_of_guests].ID = num_of_guests;
		p_guest_arr[num_of_guests].room_number = -1;
		p_guest_arr[num_of_guests].status = GUEST_WAIT;
		p_guest_arr[num_of_guests].total_number_of_nights = 0;

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

int getGuestDataFromLine(char *line, char guest_name[], int *budget) {
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