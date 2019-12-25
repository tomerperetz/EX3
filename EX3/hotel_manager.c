#include "hotel_manager.h"


// Global variables -------------------------------------------------------------------------------------->
Room_struct room_arr[MAX_NUM_OF_ROOMS];
static HANDLE room_mutex[MAX_NUM_OF_ROOMS];
static HANDLE file_mutex;
int day_counter = 1;
int num_of_guests = 0;
int num_of_rooms = 0;
int guest_counter = 0;

// Functions ------------------------------------------------------------------------------------------------------>

int checkWaitCodeStatus(DWORD wait_code) {
	int retVal1 = ERR;
	DWORD errorMessageID;
	switch (wait_code)
	{
	case WAIT_TIMEOUT:
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("details: Timeout error when waiting\n");
		break;
	case WAIT_FAILED:
		errorMessageID = GetLastError();
		printf("%d\n", errorMessageID);
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("details: Timeout error when waiting\n");
		break;
	case WAIT_OBJECT_0:
		retVal1 = TRUE;
		break;
	case WAIT_ABANDONED_0:
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("details: WAIT ANDONED\n");
		break;
	}
	// special case that also allowed
	if (wait_code >= WAIT_OBJECT_0 && wait_code <= WAIT_OBJECT_0 + num_of_guests - 1) {
		retVal1 = TRUE;
	}
	return retVal1;
}

int createProgramMutexes()
{
	int retVal = TRUE;
	// initialization
	for (int i = 0; i < MAX_NUM_OF_ROOMS; i++) {
		room_mutex[i] = NULL;
	}
	file_mutex = NULL;
	file_mutex = CreateMutex(NULL, FALSE, NULL);
	if (file_mutex == NULL) {
		retVal = ERR;  goto Main_cleanup;
	}
	// Create mutexes
	for (int i = 0; i < MAX_NUM_OF_ROOMS; i++)
	{
		room_mutex[i] = CreateMutex(
			NULL,   /* default security attributes */
			FALSE,	/* don't lock mutex immediately */
			NULL); /* un-named */
		if (room_mutex[i] == NULL) {
			retVal = ERR;
			goto Main_cleanup;
		}
	}
Main_cleanup:
	if (retVal == ERR) {
		raiseError(6, __FILE__, __func__, __LINE__, "Mutex creation failed\n");
	}
	return retVal;
}

int runHotelWithThreads()
{
	extern int num_of_guests;
	extern int guest_counter;
	/*
	Description: read a list of files content into variables using threads.
	parameters:
			 - char **files_list - files list.
			 -  int *grades_list - empty grades array.
	Return: TRUE if succeded, ERR o.w
	*/
	//HANDLE p_thread_handles[MAX_FILES];
	HANDLE p_thread_handles[MAX_NUM_OF_GUESTS];
	DWORD p_thread_ids[MAX_NUM_OF_GUESTS];
	DWORD num_of_threads = MAX_NUM_OF_GUESTS;
	DWORD wait_code;
	DWORD exit_code;
	BOOL ret_val;
	int retVal1 = TRUE;
	guest_counter = num_of_guests;
	/* initialization of the thread array */
	for (int i = 0; i < MAX_NUM_OF_GUESTS; i++) {
		p_thread_handles[i] = NULL;
	}
	/* Create mutex */
	retVal1 = createProgramMutexes();
	if (retVal1 != TRUE)
		goto Main_Cleanup_2;

	/* Create thread */
	for (int i = 0; i < num_of_guests; i++)	{
		/* Prepare parameters for thread */

		p_thread_handles[i] = CreateThreadSimple((LPTHREAD_START_ROUTINE)hotelManager, (LPVOID) i, &p_thread_ids[i]);

		if (NULL == p_thread_handles[i])
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("details: Error when creating thread\n");
			retVal1 = ERR;
			goto Main_Cleanup_2;
		}
	}
	/* Wait */
	wait_code = WaitForMultipleObjects(num_of_guests, p_thread_handles, TRUE, INFINITE);

	/* WAIT CODE cases*/
	retVal1 = checkWaitCodeStatus(wait_code);
	// Checks if the wait code was WAIT_OBJECT_0, 
	if (retVal1 == ERR) {
		retVal1 = ERR;
		goto Main_Cleanup_2;
	}
	/* Check the DWORD returned by readGradeFileThread */
	for (int i = 0; i < num_of_guests; i++)
	{
		ret_val = GetExitCodeThread(p_thread_handles[i], &exit_code);
		if (FALSE == ret_val)
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("details: Error getting thread exit code\n");
			retVal1 = ERR;
			goto Main_Cleanup_2;
		}
		// checks exit code
		if (exit_code == ERR)
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("Error in thread: %d. Exit code: %d\n", i, exit_code);
			retVal1 = ERR;
			goto Main_Cleanup_2;
		}
		if (exit_code != TRUE)
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("Details: Error in thread: %d. Exit code: %d\n", i, exit_code);
			retVal1 = ERR;
			goto Main_Cleanup_2;
		}
		/* Close thread handle */
		ret_val = CloseHandle(p_thread_handles[i]);
		p_thread_handles[i] = NULL;
		if (ret_val == FALSE)
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("Details: Error when closing thread\n");
			retVal1 = ERR;
			goto Main_Cleanup_2;
		}
	}
	Main_Cleanup_2:
		closeHandles(room_mutex, MAX_NUM_OF_ROOMS);
		closeHandles(p_thread_handles, num_of_guests);
	return retVal1;
}

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
	if (fclose(fp) != FALSE) {
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

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	/*
	Description: create thread with arg wrapper
	parameters:
			- LPTHREAD_START_ROUTINE p_start_routine
			- LPVOID p_thread_parameters
			- LPDWORD p_thread_id
	Return: thread_handle if succeded, ERR o.w
	*/

	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("Details: Error when creating a thread");
		printf("\tReceived null pointer");
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("Details: Error when creating a thread");
		printf("\tReceived null pointer");
		return NULL;
	}

	thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		p_thread_parameters, /*  argument to thread function */
		0,                   /*  use default creation flags */
		p_thread_id);        /*  returns the thread identifier */

	return thread_handle;
}


/*
===================================================================================================================
											File handler - START
===================================================================================================================
*/

int writeMessegeBuffer(Guest_struct *p_guest, char *buffer, int max_size, char *mode)
{
	extern int day_counter;
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];


	sprintf_s(buffer, max_size, "%s %s %s %d\n", room_arr[p_guest->room_number].name, p_guest->name, mode, day_counter);
	return TRUE;
}

int fileExist(char *filename)
{
	struct stat buffer;
	if (stat(filename, &buffer) == 0)
		return FALSE;
	return TRUE;
}

int writeToLog(char *output_dir_path, char *content)
{
	FILE *fp = NULL;
	char *file_path = NULL;
	char file_name[12] = "roomLog.txt";
	int file_exist = 0;
	if (strcatDynamic(output_dir_path, file_name, &file_path) != TRUE)
		return ERR;

	// Checks whether this is the first time that the program trys to open the final grades file
	file_exist = fileExist(file_path);
	if (file_exist)
	{
		// open the file saflly with 'w' mode --> fp points to the begining of the file
		if (fopen_s(&fp, file_path, "w") != FALSE || fp == NULL) {
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			str_safe_free(file_path);
			return ERR;
		}
	}
	else {
		// open the file saflly with 'a' mode --> fp points to the end of the file

		if (fopen_s(&fp, file_path, "a") != FALSE || fp == NULL) 
		{
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			str_safe_free(file_path);
			return ERR;
		}
	}
	// Free memory allocated to path

	str_safe_free(file_path);
	

	// Prints content to log file
	fprintf_s(fp, content);

	// closes the file safely
	if (fclose(fp) != FALSE) {
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		return ERR;
	}


	return TRUE;
}

int logManager(Guest_struct *p_guest, char *output_dir_path, char *mode)
{

	int max_size = MAX_ROOM_NAME_LEN + MAX_GUEST_NAME_LEN + 10;
	char buffer[MAX_ROOM_NAME_LEN + MAX_GUEST_NAME_LEN + 10];
	DWORD wait_code;
	int release_code =TRUE;

	wait_code = WaitForSingleObject(file_mutex, INFINITE);
	if (checkWaitCodeStatus(wait_code) != TRUE)
		return ERR;
	
	writeMessegeBuffer(p_guest, buffer, max_size, mode);
	if (writeToLog(output_dir_path, buffer) == ERR)
		return ERR;
	release_code = ReleaseMutex(file_mutex);
	if (release_code == FALSE)
		return ERR;

	return TRUE;
}


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

void updateBudget(Guest_struct *p_guest)
{
	/* decrease customer budget */
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	p_guest->budget = p_guest->budget - room_arr[p_guest->room_number].price_pp;
}

void updateRoomAvaiabilty(Guest_struct *p_guest)
{
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	
	/* Update for tonight */
	room_arr[p_guest->room_number].availablity--;
	if (p_guest->budget==0)
	{
		room_arr[p_guest->room_number].next_day_availablity++;
		updateCustomerStatus(p_guest, GUEST_CHECK_OUT);
	}
}

void updateCustomerStatus(Guest_struct *p_guest, int status)
{
	p_guest->status = status;
}

void guestCheckInProcedure(Guest_struct *p_guest)
{
	/* Update: room avaiablity, room avaiablity next day, customer budget, customer status */
	/* update budget */
	updateBudget(p_guest);
	updateCustomerStatus(p_guest, GUEST_REGISTERED);
	updateRoomAvaiabilty(p_guest);
}

int registerRoomParllel(Guest_struct *p_guest, int room_idx)
{
	DWORD wait_code;
	int release_code = TRUE, ret_val = FALSE;

	wait_code = WaitForSingleObject(room_mutex[room_idx], INFINITE);
	if (checkWaitCodeStatus(wait_code) != TRUE)
		return ERR;
	// Critic Code
	ret_val = (isRoomAvailableWrapper(p_guest));

	release_code = ReleaseMutex(room_mutex[room_idx]);
	if (release_code == FALSE) {
		printf("release code is 0");
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
		return ERR;
	}
	return ret_val;
}

int isRoomAvailableWrapper(Guest_struct *p_guest)
{
	/* check if room currently avaiable */
	if (isRoomAvaiable(room_arr[p_guest->room_number]))
	{
		/* Check customer in */
		guestCheckInProcedure(p_guest);
		return TRUE;
	}
	return FALSE;
}

int checkIn(Guest_struct *p_guest)
{
	int ret_val = ERR;
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
	ret_val = registerRoomParllel(p_guest, p_guest->room_number);
	return ret_val;
}


/*
===================================================================================================================
											One more night - START
===================================================================================================================
*/

void CheckBudget(Guest_struct *p_guest)
{
	/*  if he has no more budget update status and availabilty for next day */
	if (p_guest->budget == 0)
	{
		room_arr[p_guest->room_number].next_day_availablity++;
		updateCustomerStatus(p_guest, GUEST_CHECK_OUT);
	}
}

void oneMoreNight(Guest_struct *p_guest)
{
	updateBudget(p_guest);
	CheckBudget(p_guest);
}


/*
===================================================================================================================
											Checkout - START
===================================================================================================================
*/

void checkOut(Guest_struct *p_guest)
{
	updateCustomerStatus(p_guest, GUEST_LEFT);
}
/*
===================================================================================================================
											EndOfDay - START
===================================================================================================================
*/
void endOfDay(Guest_struct *p_guest)
{
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	extern int day_counter;
	extern int num_of_rooms;
	extern int guest_counter;

	/* Update Availabilty */
	for (int i = 0; i < num_of_rooms; i++)
	{
		room_arr[i].availablity += room_arr[i].next_day_availablity;
		/* reset next day availabilty */
		room_arr[i].next_day_availablity = 0;
	}		
	/* decrease the number of guests if necessery */
	if (p_guest->status == GUEST_LEFT)
		guest_counter--;
	/* increase day counter */
	day_counter++;

}

/*
===================================================================================================================
											Manager demo - no threads - START
===================================================================================================================
*/

int hotelManager(LPVOID idx)
{
	char mode_in[3] = "IN";
	char mode_out[4] = "OUT";
	extern int day_counter;
	extern char **g_argv;
	extern Guest_struct guest_arr[MAX_NUM_OF_GUESTS];
	int guest_idx = 0, ret_val = ERR;
	guest_idx = (int)idx;


	switch (guest_arr[guest_idx].status)
	{
		case (GUEST_WAIT):
		{
			ret_val = checkIn(&guest_arr[guest_idx]);
			if (ret_val)
			{
				if (!logManager(&guest_arr[guest_idx], g_argv[1], mode_in))
					return ERR;
			}
			else if (ret_val == ERR)
				return ERR;
			break;
		}
		case (GUEST_REGISTERED):
		{
			oneMoreNight(&guest_arr[guest_idx]);
			break;
		}
		case (GUEST_CHECK_OUT):
		{
			checkOut(&guest_arr[guest_idx]);
			if (!logManager(&guest_arr[guest_idx], g_argv[1], mode_out))
				return ERR;
			break;
		}
		case (GUEST_LEFT):
		{
			break;
		}
			
	}
	return TRUE;
}