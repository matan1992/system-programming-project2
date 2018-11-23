//Description – This module holds all the functions that are related to files (read and write files) and in addition a free allocation function
#include "WriteTests.h"

/*
Description – This function writes to the results file according to the reults
from the threads and the proccessess that were opened.
Parameters – path to the write file, number of threads,test_data_arr .
Returns – Void. writes the massage to the results write file.
*/
int WriteToResultsFile(char *write_file_path, int num_threads, test_data test_data_arr[])
{
	int i = 0, j = 0, compare_files_result;
	char msg_to_write[STR_MAX_LEN];
	FILE *write_file = fopen(write_file_path, "w");
	if (NULL == write_file)
	{
		printf("Failed to open write file.\n");
		return ERROR_CODE;
	}
	for (i = 0; i < num_threads; i++)
	{
		j = i + 1;//test index is i+1
		if (true == test_data_arr[i].timeout_flag)
		{
			//print timeout result
			sprintf(msg_to_write, "test #%d : Timed Out\n", j);
			WriteMsgToFile(write_file, msg_to_write);
		}
		else
		{
			if (SUCCESS == test_data_arr[i].result)
			{
				//if we got here that means that the process has produced some output file - need to see if it matches the result
				compare_files_result = compareFiles(test_data_arr[i]);
				if (ERROR_CODE == compare_files_result)
				{
					//in case one of the files doesn't exist
					fclose(write_file);
					remove(write_file_path);
					return ERROR_CODE;
				}
				if (MISMATCH == compare_files_result) // compare expected_results and the output files and check if they match
				{
					sprintf(msg_to_write, "test #%d : Failed\n", j);
					WriteMsgToFile(write_file, msg_to_write);
				}
				else
				{
					sprintf(msg_to_write, "test #%d : Succeeded\n", j);
					WriteMsgToFile(write_file, msg_to_write);
				}
			}
			else
			{
				sprintf(msg_to_write, "test #%d : Crashed %d\n", j, test_data_arr[i].result);
				WriteMsgToFile(write_file, msg_to_write);
			}
		}
	}
	fclose(write_file);
	return SUCCESS;
}

/*
Description – This function writes messege to  the plan file.
Parameters – file type of the write file, massage which is the line to write.
Returns – Void. writes the massage to the write file.
*/

void WriteMsgToFile(FILE *write_file, char *msg)
{
	errno_t retval;
	if (NULL == write_file)
	{
		printf("Failed to open write file.\n");
		exit(ERROR_CODE);
	}
	retval = fputs(msg, write_file);
	if (0 > retval)
	{
		printf("Failed to write to file.\n");
		// Don't return. Try and close the file first.
		retval = fclose(write_file);
		if (0 != retval)
		{
			printf("Failed to close file.\n");
			exit(ERROR_CODE);
		}
		exit(ERROR_CODE);
	}
}

/*
Description – function to compare two files and report
Parameters – test_dat struct - the struct that contains all files names that need to be open
Returns – int. MISMATCH if the files don't match or SUCCESS if fles match. ERROR_CODE in case of failure in opening one of the files
*/
int compareFiles(test_data test_struct)
{
	//open files
	FILE *fp1 = fopen(test_struct.expected_results, "r");
	if (fp1 == NULL)
	{
		printf("Failed to open expected results file.\n");
		return ERROR_CODE;
	}
	FILE *fp2 = fopen(test_struct.result_file, "r");
	if (fp2 == NULL)
	{
		printf("Failed to open result file.\n");
		fclose(fp1);
		return ERROR_CODE;
	}
	// fetching character of two file 
	// in two variable ch1 and ch2 
	char ch1 = getc(fp1);
	char ch2 = getc(fp2);
	//open files

	// iterate loop till end of file 
	while (ch1 != EOF && ch2 != EOF)
	{
		// if fetched data is not equal then 
		// return MISMATCH
		if (ch1 != ch2)
		{
			return MISMATCH;
		}
		// fetching character until end of file 
		ch1 = getc(fp1);
		ch2 = getc(fp2);
	}
	if (ch1 != ch2)
	{
		return MISMATCH;
	}
	// closing both file 
	fclose(fp1);
	fclose(fp2);
	return SUCCESS;
}

void FreeAllAllocatedMemory(test_data *test_data_arr, HANDLE *p_thread_handles, DWORD *p_thread_ids)
{
	free(test_data_arr);
	free(p_thread_handles);
	free(p_thread_ids);
}