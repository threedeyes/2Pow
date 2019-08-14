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

