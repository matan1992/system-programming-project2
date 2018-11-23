//Description – This module holds all the includes and defintions and name defintions
//it also include the test_data struct that includes all the data of each test.
#pragma once

#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <tchar.h> /* for TCHAR, _T() */

#define STR_MAX_LEN 100
#define ERROR_CODE -1
#define SUCCESS 0
#define TIMEOUT_IN_MILLISECONDS 10000 //Timeout is defined to 10 seconds
#define BRUTAL_TERMINATION_CODE 0x55
#define NO_SUCH_PROCESS -40

//main struct for this project. holds values which indicate the test features and results.
typedef struct test_data_s
{
	int index; //thread number (in line)
	int result; //thread status after run
	bool timeout_flag; //this flag indicates if the test timed out
	char command[STR_MAX_LEN]; //exe file name and arguments 
	char expected_results[STR_MAX_LEN]; //expected results file name
	char result_file[STR_MAX_LEN]; //output file name .
}test_data;

//functions declerations
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
DWORD WINAPI CreateProcessSimpleMain(LPVOID lpParam);