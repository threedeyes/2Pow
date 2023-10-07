/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _2POW_APPLICATION_H
#define _2POW_APPLICATION_H

#include <Application.h>
#include "MainWindow.h"

#define APPSIGNATURE "application/x-vnd.twopow-game"

class B2PowApplication : public BApplication {
	public:
						B2PowApplication();
		virtual			~B2PowApplication();

	private:
		MainWindow		*window;
};

#endif

