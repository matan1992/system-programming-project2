//Description – This module open <path to test file> and extracts the exe file name and it's directory (if exists)
//              all the argument of the test, and the file we need to compare with - expected results.
//              It fills all this data into the fields of the struct "test_data", and handles all the changes need
//				to be done on the data before filling it into the fields.

#include "ReadTests.h"
/*
• Description –calculates the number of lines in the input file (the number of threads).
• Parameters – pointer to the head of the file .
• Returns – number of threads, or - if failed to open file - -1.
*/
int ThreadsCounter(char *file_name) {
	int num_of_threads = 0;
	char ch;
	FILE *test_file = NULL;
	test_file = fopen(file_name, "r");
	if (NULL == test_file)
	{
		printf("Failed to open file.\n");
		exit(ERROR_CODE);
	}
	do
	{
		ch = fgetc(test_file);
		if (ch == '\n')
			num_of_threads++;
	} while (ch != EOF);
	fclose(test_file);
	return num_of_threads;
}

/*
• Description – extracts the last word of a string.
• Parameters – pointer to string which contains a line of the input file.
• Returns – the last word of the string given. this word is the expected_results.txt file.
*/
char *GetLastWord(char *line)
{
	char *last_word = NULL;
	last_word = strrchr(line, ' ');
	last_word = strtok(last_word, " ");
	return last_word;
}

 /*
• Description – extracts data from input file, and fills the "test_data" type array which contains all the data for each test.
• Parameters – pointer to string which contains a line of the input file, a pointer to "test_data" type.
• Returns – nothing. only fills the "test_data" type fileds, according to the data extracted from line.
*/
void GetDataFromLine(char *line, test_data *test_info)
{
	char *word = NULL, *last_word = NULL, *line_temp = NULL;
	strcpy(test_info->command, "");//initialize command to an empty string
	strcpy(test_info->expected_results, "");//initialize exp_res to an empty string
	strtok(line, "\n");//get rid of the end of line back slash
	line_temp = (char *)malloc(sizeof(char)* (STR_MAX_LEN + 1)); 
	if(NULL ==line_temp)
	{
		printf("Memory Allocation Failed\n");
		exit(ERROR_CODE);
	}
	strcpy(line_temp, line);
	last_word = GetLastWord(line); //call a function which extracts the last word and save it
	word = strtok(line, " ");// extract the first word (exe)
	strcat(test_info->command, word);//fill the right format of exe in command field
	strtok(line_temp, " ");//avoiding strtok for word from becoming NULL
	while (strcmp(word, last_word) != 0)
	{
		//get command
		word = strtok(NULL, " ");
		if (strcmp(word, last_word) != 0)//as long as current word didn't reach last
		{
			strcat(test_info->command, " "); //add white space between test name and args, as long it is not the last arg
			strcat(test_info->command, word);
		}
	}
	//get expected_result
	strcpy(test_info->expected_results,last_word);
	//free allocated memory
	free(line_temp);
	return;
}
/*
• Description – converts suffix of file, to "txt" and removes path from file.
• Parameters – pointer to "test_data" type.
• Returns – nothing. only fills the field "result_file" of the struct
*/
void ConvertExeToTxt(test_data *test)
{
	char *txt_file = NULL;
	txt_file = (char *)malloc(sizeof(char) * (strlen(test->command) + 1)); 
	if (NULL == txt_file)
	{
		printf("Memory Allocation Failed\n");
		exit (ERROR_CODE);
	}
	strcpy(txt_file, test->command);
	strtok(txt_file, " ");

	//changing "exe" to "txt"
	txt_file[strlen(txt_file) - 3] = 't';
	txt_file[strlen(txt_file) - 1] = 't';

	strcpy(test->result_file,txt_file);
	free(txt_file); //free allocated memory
	return;
}
/*
• Description – opens <path to test file>, 
  and calls all the relevant functions to fill all the fields of the "test_data" type 
• Parameters – pointer to "test_data" type.
• Returns – nothing. only fills the field "result_file" of the struct
*/
test_data *TestsRead(char *file_name,int *num_of_threads)
{
	int i = 0;
	FILE *test_file = NULL;
	char line[STR_MAX_LEN];
	(*num_of_threads) = ThreadsCounter(file_name);
	test_data *test_data_arr = NULL;
	test_data_arr = (test_data *)malloc(sizeof(test_data) * (*num_of_threads));
	if (NULL == test_data_arr)
	{
		printf("Memory Allocation Failed\n");
		exit(ERROR_CODE);
	}
	test_file = fopen(file_name, "r");
	if (NULL == test_file)
	{
		printf("Failed to open file.\n");
		free(test_data_arr);
		exit(ERROR_CODE);
	}
	while (i < *num_of_threads && NULL != fgets(line, STR_MAX_LEN, test_file))
	{
		GetDataFromLine(line, &(test_data_arr[i]));
		ConvertExeToTxt(&(test_data_arr[i]));
		test_data_arr[i].index = i + 1;
		i++;
	}
	fclose(test_file);
	return test_data_arr;
}