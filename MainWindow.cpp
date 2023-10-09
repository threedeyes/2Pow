#include <stdio.h>
#include "Application.h"
#include "MainWindow.h"

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "MainWindow"

MainWindow::MainWindow(BRect frame, const char* title)
	: BWindow(frame, title, B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{	
	fMenuBar = new BMenuBar(BRect(0, 0, Bounds().Width(), 22), "menubar");
	fMenuGame = new BMenu(B_TRANSLATE("Game"));
	fMenuGameSize = new BMenu(B_TRANSLATE("Board size"));
	fMenuGameSize->AddItem(new BMenuItem(B_TRANSLATE("4 × 4"), new BMessage(kBoardSize4x4Msg)));
	fMenuGameSize->AddItem(new BMenuItem(B_TRANSLATE("5 × 5"), new BMessage(kBoardSize5x5Msg)));
	fMenuGameSize->AddItem(new BMenuItem(B_TRANSLATE("6 × 6"), new BMessage(kBoardSize6x6Msg)));
	fMenuHelp = new BMenu(B_TRANSLATE("Help"));
	fMenuGame->AddItem(new BMenuItem(B_TRANSLATE("New game"), new BMessage(kNewGameMsg), 'N'));
	fMenuGame->AddItem(fMenuGameSize);
	fMenuGame->AddSeparatorItem();
	fMenuGame->AddItem(new BMenuItem(B_TRANSLATE("Quit"), new BMessage(kQuitMsg), 'Q'));
	fMenuBar->AddItem(fMenuGame);
	fMenuGame->SetTargetForItems(this);

	fMenuHelp->AddItem(new BMenuItem(B_TRANSLATE("About 2Pow"), new BMessage(kAboutMsg)));
	fMenuBar->AddItem(fMenuHelp);
	fMenuHelp->SetTargetForItems(this);

	AddChild(fMenuBar);

	BRect boardViewRect = Bounds();
	boardViewRect.top = fMenuBar->Frame().bottom + 1;
	fBoardView = new BoardView(boardViewRect);
	AddChild(fBoardView);

	fGameManager = new GameManager();
	fBoardView->SetGameManager(fGameManager);

	SetSizeLimits(360, 2048, 390, 2048);

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
	BAboutWindow* wind = new BAboutWindow(B_TRANSLATE("About"), APPSIGNATURE);
	wind->AddCopyright(2014, "Gerasim Troeglazov (3dEyes**)");
	wind->AddDescription(B_TRANSLATE("Add the same-numbered tiles together and get to 2048!\n\n"
		"Based on the game '2048' by Gabriele Cirulli."));
	wind->Show();
}

void
MainWindow::_loadSettings(void)
{
	BPath path;
	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
		path.Append("2Pow");
		BFile file(path.Path(), B_READ_ONLY);

		if (file.InitCheck() != B_OK || file.Lock() != B_OK) {
			fGameManager->Init(4, 4);
			_updateMenu();
			return;
		}

		BRect _windowRect(100, 100, 100 + 512, 100 + 512);
		int32 _highScore = 0;
		int32 _score = 0;
		int32 _status = GAME_PLAY;
		int32 _rows = 4;
		int32 _cols = 4;

		file.ReadAttr("WindowRect", B_RECT_TYPE, 0, &_windowRect, sizeof(BRect));
		file.ReadAttr("HighScore", B_INT32_TYPE, 0, &_highScore, sizeof(int32));
		file.ReadAttr("Score", B_INT32_TYPE, 0, &_score, sizeof(int32));
		file.ReadAttr("Status", B_INT32_TYPE, 0, &_status, sizeof(int32));
		file.ReadAttr("Rows", B_INT32_TYPE, 0, &_rows, sizeof(int32));
		file.ReadAttr("Cols", B_INT32_TYPE, 0, &_cols, sizeof(int32));

		int32 *_tileArray = new int32[_rows * _cols];
		memset(_tileArray, 0, sizeof(int32) * _cols * _rows);
		file.ReadAttr("Tiles", B_INT32_TYPE, 0, _tileArray, sizeof(int32) * _rows * _cols);

		fGameManager->Init(_rows, _cols);
		_updateMenu();

		for (int row = 0; row < _rows; row++) {
			for (int col = 0; col < _cols; col++) {
				int32 value = _tileArray[col * _cols + row];
				if (value > 0) {
					Tile *tile = new Tile(row, col, value);
					fGameManager->TileSet()->AddItem(tile);
				}
			}
		}

		delete [] _tileArray;

		MoveTo(_windowRect.left, _windowRect.top);
		ResizeTo(_windowRect.Width(), _windowRect.Height());

		fGameManager->SetStatus(_status);
		fGameManager->SetScore(_score);
		fGameManager->SetHighScore(_highScore);

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
		int32 _highScore = fGameManager->HighScore();
		int32 _status = fGameManager->Status();
		int32 _rows = fGameManager->Rows();
		int32 _cols = fGameManager->Cols();

		file.WriteAttr("WindowRect", B_RECT_TYPE, 0, &_windowRect, sizeof(BRect));
		file.WriteAttr("Score", B_INT32_TYPE, 0, &_score, sizeof(int32));
		file.WriteAttr("HighScore", B_INT32_TYPE, 0, &_highScore, sizeof(int32));
		file.WriteAttr("Status", B_INT32_TYPE, 0, &_status, sizeof(int32));
		file.WriteAttr("Rows", B_INT32_TYPE, 0, &_rows, sizeof(int32));
		file.WriteAttr("Cols", B_INT32_TYPE, 0, &_cols, sizeof(int32));

		int32 *_tileArray = new int32[_cols * _rows];
		memset(_tileArray, 0, sizeof(int32) * _cols * _rows);
		Tile *_tileItem;
		for (int32 i = 0; _tileItem = (Tile*)fGameManager->TileSet()->ItemAt(i); i++)
			_tileArray[_tileItem->Col() * _cols + _tileItem->Row()] = _tileItem->Value();

		file.WriteAttr("Tiles", B_INT32_TYPE, 0, _tileArray, sizeof(int32) * _cols * _rows);
		delete [] _tileArray;

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
MainWindow::_updateMenu(void)
{
	fMenuGameSize->FindItem(kBoardSize4x4Msg)->SetMarked(fGameManager->Cols() == 4);
	fMenuGameSize->FindItem(kBoardSize5x5Msg)->SetMarked(fGameManager->Cols() == 5);
	fMenuGameSize->FindItem(kBoardSize6x6Msg)->SetMarked(fGameManager->Cols() == 6);
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
			
			if(key == KEY_LEFT)
				tileDirection = TILE_MOVE_LEFT;
			if(key == KEY_RIGHT)
				tileDirection = TILE_MOVE_RIGHT;
			if(key == KEY_UP)
				tileDirection = TILE_MOVE_TOP;
			if(key == KEY_DOWN)
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
			break;
		}
		case kAboutMsg:
		{
			About();
			break;
		}
		case kNewGameMsg:
		{
			fGameManager->Restart();
			fGameManager->NewTile();
			fGameManager->NewTile();
			_animateTiles();
			break;
		}
		case kBoardSize4x4Msg:
		{
			fGameManager->Init(4, 4);
			_updateMenu();
			fGameManager->NewTile();
			fGameManager->NewTile();
			_animateTiles();
			break;
		}
		case kBoardSize5x5Msg:
		{
			fGameManager->Init(5, 5);
			_updateMenu();
			fGameManager->NewTile();
			fGameManager->NewTile();
			_animateTiles();
			break;
		}
		case kBoardSize6x6Msg:
		{
			fGameManager->Init(6, 6);
			_updateMenu();
			fGameManager->NewTile();
			fGameManager->NewTile();
			_animateTiles();
			break;
		}
		case kQuitMsg:
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
