//Authors – Matan Nahamiev 203499942 Sharon Rabinoviz 203039185
//Project – ex2
//Description – main module. Call all the relevent functions to init the test_data_arr and create the threads 
//				that check all tests and their result and write to the results file. reutrn SUCCESS when all functions finished

#include "CreateProcess.h"
#include "CreateThread.h"
#include "ReadTests.h"

int main(int argc, char *argv[])
{
	int num_threads;
	test_data *test_data_arr = NULL;
	test_data_arr = TestsRead(argv[1], &num_threads);
	RunAndCheckAllTests(num_threads, test_data_arr, argv[2]);
	return SUCCESS;
}