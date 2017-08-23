//============================================================================
// Name        : main.cpp
// Author      : !!M -  Rom#1
// Version     :
// Copyright   : Blabla
// Description : No Description :D
//
//============================================================================
#include "NNMReaderFirst.h"
#include <windows.h>
#include <string.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	NNMReaderFirst *app = NULL;
	char* s = NULL;

	try
	{
		bool	fullscreen	= false;
		if(MessageBox(NULL, "Would you like to run in f-f-ffullscreen mode?", "<3", MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			fullscreen	= true;
		}

		// Get the command line argument
		if(strstr(lpCmdLine, (char*)" ") != NULL)
		{
			s = (char*)malloc(strlen(lpCmdLine)-1);
			strncpy(s, lpCmdLine + 1, strlen(lpCmdLine)-3);
			s[strlen(lpCmdLine)-3] = '\0';
			printf(s);
		}

		if(s != NULL) app = new NNMReaderFirst(fullscreen, s);
		else app = new NNMReaderFirst(fullscreen, lpCmdLine);
		app->run();
	}
	catch(const std::exception& e)
	{
		if(app != NULL) delete(app);
		if(s != NULL) free(s);
		MessageBox(NULL, e.what(), "Fatal Error", MB_ICONERROR | MB_OK);
	}
	delete(app);
	if(s != NULL) free(s);
	return 0;
}
