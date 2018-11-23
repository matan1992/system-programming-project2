////Description – This module holds all the includes and functions defintions and name defintions
#pragma once

#include "CreateProcess.h"

#define MISMATCH -3

//Function declarations
void FreeAllAllocatedMemory(test_data *test_data_arr, HANDLE *p_thread_handles, DWORD *p_thread_ids);
int compareFiles(test_data test_struct);
int WriteToResultsFile(char *write_file_path, int num_threads, test_data test_data_arr[]);
void WriteMsgToFile(FILE *write_file, char *msg);
