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

	_loadSettings();

	if (fGameManager->TileSet()->CountItems() == 0) {
		fGameManager->NewTile();
		fGameManager->NewTile();
	}

	_animateTiles();
}


MainWindow::~MainWindow()
{
	_saveSettings();
}

void
MainWindow::About(void)
{
	BAboutWindow* wind = new BAboutWindow("About", APPSIGNATURE);
	wind->AddCopyright(2014, "Gerasim Troeglazov (3dEyes**)");
	wind->AddDescription(
		"Join the numbers and get to the 2048 tile!\n\n"
		"Based on 2048 game by Gabriele Cirulli.");
	wind->Show();
}

void
MainWindow::_loadSettings(void)
{
	BPath path;
	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
		path.Append("2Pow");
		BFile file(path.Path(), B_READ_ONLY);

		if (file.InitCheck() != B_OK || file.Lock() != B_OK)
			return;

		BRect _windowRect(100, 100, 100 + 512, 100 + 512);
		int32 _highScore = 0;
		int32 _score = 0;
		int32 _tileArray[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		file.ReadAttr("WindowRect", B_RECT_TYPE, 0, &_windowRect, sizeof(BRect));
		file.ReadAttr("HighScore", B_INT32_TYPE, 0, &_highScore, sizeof(int32));
		file.ReadAttr("Score", B_INT32_TYPE, 0, &_score, sizeof(int32));
		file.ReadAttr("Tiles", B_INT32_TYPE, 0, &_tileArray, sizeof(int32) * 16);

		fGameManager->Restart();
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				int32 value = _tileArray[col * 4 + row];
				if (value > 0) {
					Tile *tile = new Tile(row, col, value);
					fGameManager->TileSet()->AddItem(tile);
				}
			}
		}

		MoveTo(_windowRect.left, _windowRect.top);
		ResizeTo(_windowRect.Width(), _windowRect.Height());

		fHighScore = _highScore;
		fGameManager->SetScore(_score);

		file.Unlock();
	}
}

void
MainWindow::_saveSettings(void)
{
	BPath path;
	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
		path.Append("2Pow");

		BFile file(path.Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
		if (file.InitCheck() != B_OK || file.Lock() != B_OK)
			return;

		BRect _windowRect = Frame();
		int32 _score = fGameManager->Score();

		file.WriteAttr("WindowRect", B_RECT_TYPE, 0, &_windowRect, sizeof(BRect));
		file.WriteAttr("Score", B_INT32_TYPE, 0, &_score, sizeof(int32));
		file.WriteAttr("HighScore", B_INT32_TYPE, 0, &fHighScore, sizeof(int32));

		int32 _tileArray[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		Tile *tileItem;
		for (int32 i = 0; tileItem = (Tile*)fGameManager->TileSet()->ItemAt(i); i++)
			_tileArray[tileItem->Col() * 4 + tileItem->Row()] = tileItem->Value();

		file.WriteAttr("Tiles", B_INT32_TYPE, 0, &_tileArray, sizeof(int32) * 16);

		file.Sync();
		file.Unlock();
	}
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
