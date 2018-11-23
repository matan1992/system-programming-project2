//Description – This module holds all the includes and defintions and name defintions
#pragma once

#include "CreateProcess.h"
#include "WriteTests.h"

//funtion declerations
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE thread_function,
	LPDWORD p_thread_id, test_data *test_info);
void RunAndCheckAllTests(int num_threads, test_data test_data_arr[], char *path_to_result_file);
static void CloseAllThreads(HANDLE p_thread_handles[], int index, int num_threads);
