//Description – This module creates proccesses and according to the return value of each procces fills in the test_data
//				struct accordingly..

#include "CreateProcess.h"

/*
Description – This function will be called from a thread. It will get 
a test_data struct and fill it with the apropriate data according
to the process reutrn value.
Parameters – LPVOID lpParam. In this project it will always be called
with a test_data struct to fill the correct data in.
Returns – Void. Fills the data in the test_data struct
*/
DWORD WINAPI CreateProcessSimpleMain(LPVOID lpParam)
{
	int test_index;
	test_data *test_info;
	test_info = (test_data *)lpParam;
	test_index = test_info->index;
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	TCHAR				command[STR_MAX_LEN];
	strcpy(command, _T(test_info->command)); /* <ISP> TCHAR is a win32  */
																					/* generic char which may be either a simple (ANSI) char or a unicode char, */
																					/* depending on behind-the-scenes operating system definitions. Type LPTSTR */
																					/* is a string of TCHARs. Type LPCTSTR is a const string of TCHARs. */

																					/*  Start the child process. */
	retVal = CreateProcessSimple(command, &procinfo);
	if (retVal == 0)
	{
		printf("Process %s Creation Failed!\n", test_info->command);
		return NO_SUCH_PROCESS;
	}


	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 10 secs for the process to end */

	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		test_info->timeout_flag = true;
		break;
	case WAIT_OBJECT_0:
		test_info->timeout_flag = false;
		break;
	case WAIT_FAILED:
		test_info->timeout_flag = false;
		break;
		//nned to remove default eventually
	default:
		break;
	}
	//in case we want to terminate the process that timedout. in this project it's not neccessary
	//if (waitcode == WAIT_TIMEOUT) /* Process is still alive */
	//{
	//	TerminateProcess(
	//		procinfo.hProcess,
	//		BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of 0x55 */
	//	Sleep(10); /* Waiting a few milliseconds for the process to terminate */
	//}
	GetExitCodeProcess(procinfo.hProcess, &exitcode); // get the "return value"of the process
	// fill in test_info array the proper result
	test_info->result = (int)exitcode;

	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
	//return 0 is a default
	return SUCCESS;
}
/*
Description – This function just creates a process with the aprropriate args
Parameters – the command and the process handle
Returns – BOOL according to the process result
*/
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}

