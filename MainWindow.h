#ifndef _2048_WINDOW_H
#define _2048_WINDOW_H


#include <Window.h>
#include <View.h>
#include <OS.h>
#include <Alert.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <be/storage/Path.h>
#include <Entry.h>
#include <Directory.h>
#include <String.h>
#include <TextControl.h>
#include <ListView.h>
#include <String.h>
#include <Bitmap.h>
#include <ScrollView.h>
#include <Button.h>

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
		
		
		void About(void);	

	private:		

		void		_animateTiles(void);
		
		BMenu    	*fMenuGame;
		BMenu 	 	*fMenuHelp;
		BMenuBar 	*fMenuBar;
		BoardView 	*fBoardView;
		GameManager	*fGameManager;
};

#endif


