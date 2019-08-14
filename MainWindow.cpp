#include <stdio.h>
#include "Application.h"
#include "MainWindow.h"


MainWindow::MainWindow(BRect frame, const char* title)
	: BWindow(frame, title, B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{	
	BMenuItem *item;

	fGameManager = new GameManager();

	fBoardView = new BoardView(Bounds());	
	AddChild(fBoardView);
	
	fBoardView->SetGameManager(fGameManager);

	fMenuBar = new BMenuBar(BRect(0, 0, Bounds().Width(), 22), "menubar");
	fMenuGame = new BMenu("Game");
	fMenuHelp = new BMenu("Help");
	fMenuGame->AddItem(new BMenuItem("New", new BMessage('NEW_'), 'N'));
	fMenuGame->AddSeparatorItem();
	fMenuGame->AddItem(new BMenuItem("Quit", new BMessage('QUIT'), 'Q'));
	fMenuBar->AddItem(fMenuGame);	
	fMenuGame->SetTargetForItems(this);
	
	fMenuHelp->AddItem(new BMenuItem("About", new BMessage('ABOU')));
	fMenuBar->AddItem(fMenuHelp);		
	fMenuHelp->SetTargetForItems(this);

	fBoardView->AddChild(fMenuBar);
	
	SetSizeLimits(360, 2048, 360, 2048);
	
	SetPulseRate(5000);

	fGameManager->Restart();
	fGameManager->NewTile();
	fGameManager->NewTile();
	_animateTiles();
}


MainWindow::~MainWindow()
{	 
}

void
MainWindow::About(void)
{
	BAlert* alert = new BAlert("About", B_UTF8_COPYRIGHT" 2014-2019 Gerasim Troeglazov (3dEyes**)\n\nJoin the numbers and get to the 2048 tile!\n\nBased on 2048 game by Gabriele Cirulli.", "OK");
	alert->Go(NULL);
}

void
MainWindow::_animateTiles(void)
{
	for (int i = 0; i <= ANIMATION_STEPS; i++) {
		Tile *tileItem;
		for (int32 i = 0; tileItem = (Tile*)fGameManager->TileSet()->ItemAt(i); i++)
			tileItem->DoStep();
		fBoardView->MyDraw();
		//snooze(1000000 / (ANIMATION_STEPS * 40));
	}
}

void 
MainWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case B_UNMAPPED_KEY_DOWN:
		case B_KEY_DOWN:
		{
			const char *bytes;
			int32 key;
			message->FindInt32("key",&key);	
			
			int tileDirection = -1;
			
			if(key==97)
				tileDirection = TILE_MOVE_LEFT;
			if(key==99)
				tileDirection = TILE_MOVE_RIGHT;
			if(key==87)
				tileDirection = TILE_MOVE_TOP;
			if(key==98)
				tileDirection = TILE_MOVE_BOTTOM;
				
			if (tileDirection != -1) {
				int moveCount;
				for (moveCount = 0; fGameManager->MoveTilesTo(tileDirection); moveCount++)
					_animateTiles();
				
				if(moveCount != 0) {
					snooze(50000);
					fGameManager->NewTile();
					_animateTiles();
				}
				fBoardView->MyDraw();
			}

			/*if(key==94) {
				for (int row = 0; row < 4; row++)
					for (int col = 0; col < 4; col++) {
						if (fGameManager->TileAt(row, col) != NULL)
							fGameManager->TileAt(row, col)->SetValue(fGameManager->TileAt(row,col)->Value() * 2);
					}
				fBoardView->Invalidate();
			}*/
				
			break;
		}
		case 'ABOU':
		{
			About();
			break;
		}
		case 'NEW_':
		{
			fGameManager->Restart();
			fGameManager->NewTile();
			fGameManager->NewTile();
			_animateTiles();
			break;
		}		
		case 'QUIT':
		{
			Close();
			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
