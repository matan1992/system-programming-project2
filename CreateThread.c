//Description – This module creates a thread for each test and runs it parellely and writes to the result file the 
//				appropriate data
#include "CreateThread.h"

/*
Description – this function opens all threads with the appropriate function (this function opens a process)
			it checks the return value of all threads at handles it accordingly.
			it also checks the test_data_array and according to the data in that array
			writes to the result file	
Parameters – num of threads, test_data_arr that includes all data f each test_data struct, path_to_resut_file
Returns – Void. prints to the result file the apropriate data and frees all memmory allocation.
in case of failure just exits with negative num
*/
void RunAndCheckAllTests(int num_threads, test_data test_data_arr[], char *path_to_result_file)
{
	int j = 0;
	HANDLE *p_thread_handles = NULL;
	DWORD *p_thread_ids = NULL;
	DWORD wait_code;
	DWORD thread_exit_code;
	//allocate p_thread_handles and p_thread_ids arrays
	p_thread_handles = (HANDLE *)malloc(sizeof(HANDLE)* num_threads);
	p_thread_ids = (DWORD *)malloc(sizeof(DWORD)*num_threads);
	//check allocation failure
	if (NULL == p_thread_handles || NULL == p_thread_ids)
	{
		printf("Memmory Allocation failed");
		FreeAllAllocatedMemory(test_data_arr, p_thread_handles, p_thread_ids);
		exit(ERROR_CODE);
	}
	// Create threads.
	for(j = 0; j < num_threads; j++)
	{
		p_thread_handles[j] = CreateThreadSimple(CreateProcessSimpleMain, &p_thread_ids[j], &(test_data_arr[j]));
	}
	
	wait_code = WaitForMultipleObjects(
		num_threads,           // number of objects in array
		p_thread_handles,     // array of objects
		TRUE,       // wait for any object
		3*TIMEOUT_IN_MILLISECONDS		/*wait 30 seconds*/
		);
	
	// check wait_code:
	for (j = 0; j < num_threads; j++)
	{
		GetExitCodeThread(p_thread_handles[j], &thread_exit_code);
		if (thread_exit_code == NO_SUCH_PROCESS)
		{
			//handle the case where there is no such process:
			//free all allocated memmory, close all threads
			//and print to screen relevant msg
			printf("Process %s doesn't exist!!\nexiting...\n", test_data_arr[j].command);
			CloseAllThreads(p_thread_handles, -1, num_threads);
			FreeAllAllocatedMemory(test_data_arr, p_thread_handles, p_thread_ids);
			exit(ERROR_CODE);
		}
		else if (wait_code == WAIT_OBJECT_0 + j)
			continue;
		else if (wait_code == WAIT_ABANDONED_0 + j)
		{
			//Terminate thread and close all other threads
			CloseAllThreads(p_thread_handles, j, num_threads);
			printf("Thread %d of test %s Failed\n", j, test_data_arr[j].command);
			FreeAllAllocatedMemory(test_data_arr, p_thread_handles, p_thread_ids);
			exit(ERROR_CODE);
		}
		else if (wait_code == WAIT_TIMEOUT)
		{
			printf("thread %d of test %s Timed Out\n", j, test_data_arr[j].command);
			CloseAllThreads(p_thread_handles, j, num_threads);
			FreeAllAllocatedMemory(test_data_arr, p_thread_handles, p_thread_ids);
			exit(ERROR_CODE);
		}
	}
	if (ERROR_CODE == WriteToResultsFile(path_to_result_file, num_threads, test_data_arr))
	{
		CloseAllThreads(p_thread_handles, -1, num_threads);
		FreeAllAllocatedMemory(test_data_arr, p_thread_handles, p_thread_ids);
		exit(ERROR_CODE);
	}
	CloseAllThreads(p_thread_handles, -1, num_threads);
	FreeAllAllocatedMemory(test_data_arr,p_thread_handles,p_thread_ids);
}

/*
Description – this function closes al thread handles. in case of thread failure it gets the index of the thread that failed
			 else the function gets a negative index so that all threads will be closed
Parameters – thread handle, index to terminate a thread (negative num in case that all threads need to close
			and num of threads.
Returns – Void. closes all thredas
*/

static void CloseAllThreads(HANDLE p_thread_handles[], int index, int num_threads)
{
	int i = 0;
	BOOL ret_val;
	for (i = 0; i < num_threads; i++)
	{
		//printf("data of process/handle:\n index:%d\n result:%d\n timeout:%d\n", test_data_arr[i].index, test_data_arr[i].result, test_data_arr[i].timeout_flag);
		if (i == index)
		{
			//terminate the thread
			ret_val = TerminateThread(p_thread_handles[i], BRUTAL_TERMINATION_CODE);
			if (false == ret_val)
			{
				printf("Error when terminating Thread!\n");
				exit(ERROR_CODE);
			}
			continue;
		}		
		ret_val = CloseHandle(p_thread_handles[i]);
		if (false == ret_val)
		{
			printf("Error when closing thread\n");
			exit(ERROR_CODE);
		}
	}
}
/*
Description – This function just creates a thread with the aprropriate process
and args for the process
Parameters – thread_function that the thread will open, thread_id, struct of type test_data 
to send to the threead_function as an argument
Returns – HANDLE according to the thread reutrn value
*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE thread_function,
	LPDWORD p_thread_id, test_data *test_info)
{
	HANDLE thread_handle;

	if (NULL == thread_function || NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}
	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		thread_function, /*  thread function */
		test_info, /*  argument to thread function */ // send the correct data to the function
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread of Process:%s\n", test_info->command);
		exit(ERROR_CODE);
	}
	return thread_handle;
}