/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "Application.h"
#include "MainWindow.h"

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "Application"

B2PowApplication::B2PowApplication()
		  :BApplication(APPSIGNATURE)
{
	BRect windowRect(100, 100, 100 + 512, 100 + 512);
	window = new MainWindow(windowRect, B_TRANSLATE_SYSTEM_NAME("2Pow"));
	window->Show();
}


B2PowApplication::~B2PowApplication()
{
}

int
main(int argc, char* argv[])
{
	B2PowApplication app;
	app.Run();
	
	return 0;
}
