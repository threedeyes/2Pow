#ifndef _2048_WINDOW_H
#define _2048_WINDOW_H


#include <Window.h>
#include <View.h>
#include <OS.h>
#include <Alert.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Entry.h>
#include <File.h>
#include <Path.h>
#include <FindDirectory.h>
#include <String.h>
#include <String.h>
#include <Button.h>

#include <private/interface/AboutWindow.h>

#include "Application.h"
#include "MainWindow.h"
#include "BoardView.h"
#include "Tile.h"
#include "GameManager.h"

class MainWindow : public BWindow {
	public:
						MainWindow(BRect rect, const char* name);
		virtual			~MainWindow();

		virtual void 	MessageReceived(BMessage *message);
		bool			QuitRequested();
		
		
		void			About(void);

	private:		
		void		_loadSettings(void);
		void		_saveSettings(void);
		void		_animateTiles(void);

		BMenu    	*fMenuGame;
		BMenu 	 	*fMenuHelp;
		BMenuBar 	*fMenuBar;
		BoardView 	*fBoardView;
		GameManager	*fGameManager;
};

#endif


