/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

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
#include <Catalog.h>

#include <private/interface/AboutWindow.h>

#include "Application.h"
#include "MainWindow.h"
#include "BoardView.h"
#include "Tile.h"
#include "GameManager.h"

#define kAboutMsg			'ABOU'
#define kQuitMsg			'QUIT'
#define kNewGameMsg			'NEWG'
#define kBoardSize4x4Msg	'S4x4'
#define kBoardSize5x5Msg	'S5x5'
#define kBoardSize6x6Msg	'S6x6'

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
		void		_updateMenu(void);

		BMenu    	*fMenuGame;
		BMenu    	*fMenuGameSize;
		BMenu 	 	*fMenuHelp;
		BMenuBar 	*fMenuBar;
		BoardView 	*fBoardView;
		GameManager	*fGameManager;
};

#endif


