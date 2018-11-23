//Description – This module holds all the includes and defintions
//(global defines, library includes, test_data structure and functions declarations) relevent to ReadTests.c.
#pragma once

#include "CreateProcess.h"

//Function declarations
int ThreadsCounter(char *file_name);
char *GetLastWord(char *line);
void GetDataFromLine(char *line, test_data *test_info);
void ConvertExeToTxt(test_data *test);
test_data *TestsRead(char *file_name, int *num_of_threads);
