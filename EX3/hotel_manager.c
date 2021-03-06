#include "hotel_manager.h"


// Global variables -------------------------------------------------------------------------------------->
Room_struct room_arr[MAX_NUM_OF_ROOMS];
static HANDLE room_mutex[MAX_NUM_OF_ROOMS];
static HANDLE file_mutex;
static HANDLE end_of_day_mutex;
static HANDLE end_of_business_day;
int day_counter = 1;
int num_of_guests = 0;
int num_of_rooms = 0;
int guest_counter = 0;
int end_of_day_guest_counter = 0;
int status = TRUE;
// Functions ------------------------------------------------------------------------------------------------------>

/*
===================================================================================================================
							Parsing Data From Room File and From Guest File
===================================================================================================================
*/


void printGuestStruct(Guest_struct guest_arr[MAX_NUM_OF_GUESTS]) 
{
	/*
	Description: Print guest struct. for debugging purposes.
	parameters:
			 - Guest_struct guest_arr[MAX_NUM_OF_GUESTS] - guest struct array
	Returns:void
	*/
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
	/*
	Description: Print Room struct. for debugging purposes.
	parameters:
			 - none. room struct is global variable.
	Returns:void
	*/
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
	/*
	Description: read room list file and init room structs.
	parameters:
			 - char dir_path[] - input dir path recieved from user.
	Returns: TRUE if succeded, ERR o.w
	*/
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
	while (!feof(fp) && num_of_rooms <= MAX_NUM_OF_ROOMS) {
		fgets(line, MAX_LINE_LENGTH, fp);
		if (strlen(line) <= MIN_LINE_LEN) {
			break;
		}
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
		strcpy_s(line, MIN_LINE_LEN, "\0");
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
	/*
	Description: parse line aed from file to variables.
	parameters:
			 - char *line - line from file
			 - char room_name[] - empty room variable to init
			 - int *price - pointer to price int
			 - int *capacity - pointer to int capcity
	Returns: TRUE if succeded, ERR o.w
	*/
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
	/*
	Description: read guest file and init guest struct
	parameters:
			 - char dir_path[] - input dir path recieved from user.
			 - Guest_struct guest_arr[MAX_NUM_OF_GUESTS] - array of guest struct variables
	Returns: TRUE if succeded, ERR o.w
	*/
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
		if (strlen(line) <= MIN_LINE_LEN) {
			break;
		}
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
		strcpy_s(line, MIN_LINE_LEN, "\0");
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
	/*
	Description: parse line from file to destination variables.
	parameters:
			 - char *line - line from file
			 - char guest_name[] - empty guest name variable to init
			 - int *budget - pointer to budget int
	Returns: TRUE if succeded, ERR o.w
	*/
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

/*
===================================================================================================================
							Threads, Mutexes and Semaphores handler - START
===================================================================================================================
*/

int checkWaitCodeStatus(DWORD wait_code, BOOL singleNotMultiple) {
	/*
	Description: check wait code status from waitForMultipleObject o rwaitForSingleObject function
	parameters:
			 - DWORD wait_code - wait code recieved
			 - BOOL singleNotMultiple - TRUE for multiple, FALSE for single

	Returns: TRUE if succeded, ERR o.w
	*/

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
	if (singleNotMultiple == FALSE){
		if (wait_code == WAIT_OBJECT_0 + num_of_guests - 1) {
			retVal1 = TRUE;
		}
	}
	return retVal1;
}

int createProgramMutexes()
{
	/*
	Description: init global mutexes
	parameters:
			 - none
	Returns: TRUE if succeded, ERR o.w
	*/

	int retVal = TRUE;
	// initialization
	for (int i = 0; i < MAX_NUM_OF_ROOMS; i++) {
		room_mutex[i] = NULL;
	}
	file_mutex = NULL;
	end_of_day_mutex = NULL;
	end_of_day_mutex = CreateMutex(NULL, FALSE, NULL);
	if (end_of_day_mutex == NULL) {
		retVal = ERR;  goto Main_cleanup;
	}
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

int createProgramSemaphores()
{
	/*
		Description: init global semaphores
		parameters:
				 - none
		Returns: TRUE if succeded, ERR o.w
	*/
	extern num_of_guests;
	int ret_val = TRUE;
	end_of_business_day = NULL;
	end_of_business_day = CreateSemaphore(NULL, 0, num_of_guests, NULL);
	if (end_of_business_day == NULL) {
		raiseError(6, __FILE__, __func__, __LINE__, "Mutex creation failed\n");
		ret_val = ERR;
	}
	return ret_val;
}

int runHotelWithThreads()
{
	/*
	Description: run hotel manager using threades
	parameters:
			 - none
	Returns: TRUE if succeded, ERR o.w
	*/
	extern int num_of_guests;
	extern int guest_counter;

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
	retVal1 = createProgramSemaphores();
	if (retVal1 != TRUE)
		goto Main_Cleanup_1;
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
	retVal1 = checkWaitCodeStatus(wait_code, FALSE);
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
		if (exit_code == ERR) {
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_8_INTER_THREAD);
			printf("An Error occured in thread: %d. Exit code is: %d\nClosing the program and all resources...\n", i, exit_code);
			retVal1 = ERR;
			goto Main_Cleanup_2;
		}
		if (exit_code != TRUE) {
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_8_INTER_THREAD);
			printf("Details: Error in thread: %d. Exit code: %d\nClosing the program and all resources...\n", i, exit_code);
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
	if (end_of_day_mutex != NULL) CloseHandle(end_of_day_mutex);
	if (file_mutex != NULL) CloseHandle(file_mutex);
	closeHandles(room_mutex, MAX_NUM_OF_ROOMS);
	closeHandles(p_thread_handles, num_of_guests);
Main_Cleanup_1:
	if (end_of_business_day != NULL) CloseHandle(end_of_business_day);
	if (retVal1 != TRUE) return ERR;
	return day_counter;
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

void writeMessegeBuffer(Guest_struct *p_guest, char *buffer, int max_size, char *mode)
{
	/*
	Description: write message buffer using given variables
	parameters:
			 - Guest_struct *p_guest - guest messeage reffers to
			 - char *buffer - empty buffer, message will be written into
			 - int max_size - max mesage size
			 - char *mode - IN/OUT
	Returns: VOID
	*/
	extern int day_counter;
	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	sprintf_s(buffer, max_size, "%s %s %s %d\n", room_arr[p_guest->room_number].name, p_guest->name, mode, day_counter);
}

int fileExist(char *filename)
{
	/*
	Description: check if given path to a file exists.
	parameters:
			 - char *filename - path to file
	Returns: TRUE if exists
	*/
	struct stat buffer;
	if (stat(filename, &buffer) == 0)
		return FALSE;
	return TRUE;
}

int writeToLog(char *output_dir_path, char *content)
{
	/*
	Description: write guest status to log file
	parameters:
			 - char *output_dir_path - destination folder
			 - char *content - content to be written

	Returns: TRUE if succeded
	*/
	FILE *fp = NULL;
	char *file_path = NULL;
	char file_name[14] = "/roomLog.txt";
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
	/*
	Description: write guest status to log file
	parameters:
			 - Guest_struct *p_guest - guest struct
			 - char *output_dir_path - destination folder
			 - char *mode - IN/OUT
	Returns: TRUE if succeded
	*/

	int max_size = MAX_ROOM_NAME_LEN + MAX_GUEST_NAME_LEN + 10;
	char buffer[MAX_ROOM_NAME_LEN + MAX_GUEST_NAME_LEN + 10];
	DWORD wait_code;
	int release_code = TRUE, ret_val = TRUE;
	wait_code = WaitForSingleObject(file_mutex, INFINITE);
	if (checkWaitCodeStatus(wait_code, TRUE) != TRUE) {
		ret_val = ERR;
		goto Realese_And_Quit;
	}
	writeMessegeBuffer(p_guest, buffer, max_size, mode);
	if (writeToLog(output_dir_path, buffer) == ERR) {
		ret_val = ERR;
	}
Realese_And_Quit:
	release_code = ReleaseMutex(file_mutex);
	if (ret_val == ERR) return ERR;
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
	/*
	Description: find room suitable to customer budget
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: TRUE if succeded
	*/
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
	/*
	Description: Check if room is currently avaiable
	parameters:
			 - Room_struct p_room - room struct
	Returns: TRUE if succeded
	*/
	if (p_room.availablity > 0)
		return TRUE;
	return FALSE;
}

void updateBudget(Guest_struct *p_guest)
{
	/*
	Description:  decrease customer budget
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: VOID
	*/

	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	p_guest->budget = p_guest->budget - room_arr[p_guest->room_number].price_pp;
}

void updateRoomAvaiabilty(Guest_struct *p_guest)
{
	/*
	Description:  update room availabilty status
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: VOID
	*/

	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	
	/* Update for tonight */
	room_arr[p_guest->room_number].availablity--;
	if (p_guest->budget==0)
	{
		room_arr[p_guest->room_number].next_day_availablity++;
		updateCustomerStatus(p_guest, GUEST_CHECKED_IN_CHECKED_OUT);
	}
}

void updateCustomerStatus(Guest_struct *p_guest, int status)
{
	/*
	Description:  Update customer status
	parameters:
			 - Guest_struct *p_guest - guest struct
			 - int status - new status
	Returns: VOID
	*/
	p_guest->status = status;
}

void guestCheckInProcedure(Guest_struct *p_guest)
{	
	/*
	Description:  room avaiablity, room avaiablity next day, customer budget, customer statu
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: VOID
	*/
	/* update budget */
	updateBudget(p_guest);
	updateCustomerStatus(p_guest, GUEST_CHECKED_IN);
	updateRoomAvaiabilty(p_guest);
}

int registerRoomParllel(Guest_struct *p_guest, int room_idx)
{
	/*
	Description:  register guest to room. can handle different rooms parallel.
	parameters:
			 - Guest_struct *p_guest - guest struct
			 - int room_idx - room index 
	Returns: TRUE if succeded.
	*/

	DWORD wait_code;
	int release_code = TRUE, ret_val = FALSE;

	wait_code = WaitForSingleObject(room_mutex[room_idx], INFINITE);
	if (checkWaitCodeStatus(wait_code, TRUE) != TRUE) {
		ret_val = ERR;
		goto Release_And_Quit;
	}
	// Critical Code
	ret_val = (isRoomAvailableWrapper(p_guest));
Release_And_Quit:
	release_code = ReleaseMutex(room_mutex[room_idx]);
	if (ret_val == ERR) return ERR;
	if (release_code == FALSE) {
		printf("release code is 0");
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
		return ERR;
	}
	return ret_val;
}

int isRoomAvailableWrapper(Guest_struct *p_guest)
{
	/*
	Description:  a wrapper for checking in function.
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: TRUE if customer checked in.
	*/

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
	/*
	Description:  try to check in guest
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: TRUE if customer checked in.
	*/

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
	/*
	Description: Check user balance. move to check out status if he is out of money.
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: VOID
	*/

	/*  if he has no more budget update status and availabilty for next day */
	if (p_guest->budget == 0)
	{
		room_arr[p_guest->room_number].next_day_availablity++;
		updateCustomerStatus(p_guest, GUEST_CHECK_OUT);
	}
}

void oneMoreNight(Guest_struct *p_guest)
{
	/*
	Description: charge user for one more night.
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: VOID
	*/
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
	/*
	Description: update customer status to GUEST_LEFT mode.
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: VOID
	*/
	updateCustomerStatus(p_guest, GUEST_LEFT);
}
/*
===================================================================================================================
											EndOfDay - START
===================================================================================================================
*/

int checkIfAllGuestDone()
{
	/*
	Description: Check if all guests left. if so program is done.
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: TRUE if al guests are done.
	*/
	extern Guest_struct guest_arr[MAX_NUM_OF_GUESTS];
	extern int num_of_guests;
	extern int status;
	extern char **g_argv;
	char mode_in[3] = "IN";
	int num_of_guest_left = 0;
	for (int i = 0; i < num_of_guests; i++) {
		if (guest_arr[i].status == GUEST_LEFT)
			num_of_guest_left += 1;
		else if (guest_arr[i].status == GUEST_CHECKED_IN) {
			if (!logManager(&guest_arr[i], g_argv[1], mode_in)) {
				status = FALSE;
				return ERR;
			}
			guest_arr[i].status = GUEST_REGISTERED;
		}
		else if (guest_arr[i].status == GUEST_CHECKED_IN_CHECKED_OUT) {
			if (!logManager(&guest_arr[i], g_argv[1], mode_in)) {
				status = FALSE;
				return ERR;
			}
			guest_arr[i].status = GUEST_CHECK_OUT;
		}
		else if (guest_arr[i].status == ERR) {
			status = FALSE;
			return ERR;
		}

	}
	if (num_of_guest_left == num_of_guests)
		status = FALSE;
	return TRUE;
}

int endOfBusinessDay(Guest_struct *p_guest)
{
	/*
	Description: Check if business day is over - all guests are handled for today, make preperations for tmrw - update room avaiablity etc.
	parameters:
			 - Guest_struct *p_guest - guest struct
	Returns: ret val - TRUE id succeded, ERR for error.
	*/

	extern Room_struct room_arr[MAX_NUM_OF_ROOMS];
	extern int day_counter;
	extern int num_of_rooms;
	extern int guest_counter;
	extern int end_of_day_guest_counter;
	DWORD wait_code;
	LONG previous_count;
	int release_res = 0, ret_val = TRUE, num_of_guest_that_left = 0;

	wait_code = WaitForSingleObject(end_of_day_mutex, INFINITE);
	if (checkWaitCodeStatus(wait_code, TRUE) != TRUE) {
		ret_val = ERR;
		goto Error_And_Close;
	}
	
	// Critic Code

	end_of_day_guest_counter++;
	if (end_of_day_guest_counter == guest_counter) {
		for (int i = 0; i < num_of_rooms; i++) {
			room_arr[i].availablity += room_arr[i].next_day_availablity;
			/* reset next day availabilty */
			room_arr[i].next_day_availablity = 0;
		}
		ret_val = checkIfAllGuestDone();
		if (ret_val == ERR) goto Error_And_Close;
		day_counter++;
		end_of_day_guest_counter = 0;
		release_res = ReleaseSemaphore(end_of_business_day, guest_counter, &previous_count);
		if (release_res == FALSE) goto Error_And_Close;

	}

	/* End of critic Code Release Mutex */

Error_And_Close:
	if (ret_val == ERR) {
		end_of_day_guest_counter = 0;
		status = FALSE;
	}
	release_res = ReleaseMutex(end_of_day_mutex);
	if (release_res == FALSE || ret_val == ERR) { 
		ret_val = ERR; goto Error_And_Close2;
	}
	wait_code = WaitForSingleObject(end_of_business_day, INFINITE);
	if (checkWaitCodeStatus(wait_code, TRUE) != TRUE)
		return ERR;

Error_And_Close2:
	if (ret_val == ERR) {
		release_res = ReleaseSemaphore(end_of_business_day, guest_counter, &previous_count);
	}
	return ret_val;

}

/*
===================================================================================================================
											Manager demo - no threads - START
===================================================================================================================
*/

static DWORD hotelManager(LPVOID idx)
{
	/*
	Description: hotel manager - handle customre according to his status
	parameters:
			 - LPVOID idx - guest idx on guest struct arr.
	Returns: ret val - TRUE id succeded, ERR for error.
	*/

	char mode_out[4] = "OUT";
	extern int day_counter;
	extern guest_counter;
	extern char **g_argv;
	extern Guest_struct guest_arr[MAX_NUM_OF_GUESTS];
	extern int status;
	int guest_idx = 0, ret_val = ERR;
	guest_idx = (int)idx;

	while(status) {
		switch (guest_arr[guest_idx].status)
		{
			case (GUEST_WAIT):
			{
				ret_val = checkIn(&guest_arr[guest_idx]);
				if (ret_val == ERR)
					guest_arr[guest_idx].status = ERR;
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
					guest_arr[guest_idx].status = ERR;
				break;
			}
			case (GUEST_LEFT):
			{
				break;
			}	
		}
		ret_val = endOfBusinessDay(&guest_arr[guest_idx]);
		if (ret_val != TRUE) return ERR;
	}

	return ret_val;
}

