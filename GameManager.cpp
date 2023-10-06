#include "Tile.h"
#include "GameManager.h"

class Tile;

GameManager::GameManager()
{
	fTileSet = new BList();
	srand (time(NULL));	
	gameStatus = GAME_PLAY;
	fScore = 0;
	fDropStep = 0;
}


GameManager::~GameManager()
{
	Restart();	
	delete fTileSet;
}

Tile*
GameManager::TileAt(int row, int col)
{
	Tile *tileItem;
	for (int32 i = 0; tileItem = (Tile*)fTileSet->ItemAt(i); i++) {
		if(tileItem->Row() == row && tileItem->Col() == col && !tileItem->Deleted()) {
			return tileItem;
		}
	}
	return NULL;
}

int
GameManager::CheckAt(int row, int col)
{
	if (row > 3 || row < 0 || col > 3 || col < 0)
		return -1;
	Tile *tileItem = TileAt(row, col);	
	if (tileItem == NULL)
		return 0;
	if (tileItem->Merged())
		return -1;
	return tileItem->Value();
}


bool
GameManager::checkForMove(void)
{
	if (fTileSet->CountItems() < 16)
		return true;
	
	bool possible = false;
	Tile *tileItem;
	for (int32 i = 0; tileItem = (Tile*)fTileSet->ItemAt(i); i++) {
		int row = tileItem->Row();
		int col = tileItem->Col();
		int left = CheckAt(row, col - 1);
		int right = CheckAt(row, col + 1);
		int top = CheckAt(row - 1, col);
		int bottom = CheckAt(row + 1, col);
		if(left == 0 || right == 0 || top == 0 || bottom == 0)
			return true;
		if (tileItem->Value() == left ||
			tileItem->Value() == right || 
			tileItem->Value() == top || 
			tileItem->Value() == bottom)
			return true;
	}
	return false;
}



bool
GameManager::MoveTilesTo(int dir)
{	
	if (gameStatus != GAME_PLAY && gameStatus != GAME_CONT)
		return false;
	
	bool tilesMoved = false;
	
	int rowDirections[4] = {0, 0, -1, 1};
	int colDirections[4] = {-1, 1, 0, 0};
	
	int rowShift = rowDirections[dir];
	int colShift = colDirections[dir];
	
	Tile *tileItem;
		
	if (dir == TILE_MOVE_BOTTOM) {
		for (int row = 2; row >= 0; row--) {
			for (int col = 0; col < 4; col++) {
				Tile *src = TileAt(row, col);
				if (src != NULL) {
					int targetValue = CheckAt(row + rowShift, col + colShift);
					if (targetValue == 0) {
						src->MoveBy(rowShift, colShift);
						tilesMoved = true;
					} else if (targetValue == src->Value() && !src->Merged()) {
						Tile *dst = TileAt(row + rowShift, col + colShift);
						dst->SetDeleted(true);

						src->MoveBy(rowShift, colShift);
						fScore += targetValue * 2;
						src->SetMerged(true);
						tilesMoved = true;
					}
				}
			}
		}
	} else if (dir == TILE_MOVE_TOP) {
		for (int row = 1; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				Tile *src = TileAt(row, col);
				if (src != NULL) {
					int targetValue = CheckAt(row + rowShift, col + colShift);
					if (targetValue == 0) {
						src->MoveBy(rowShift, colShift);
						tilesMoved = true;
					} else if (targetValue == src->Value() && !src->Merged()) {
						Tile *dst = TileAt(row + rowShift, col + colShift);
						dst->SetDeleted(true);

						src->MoveBy(rowShift, colShift);
						fScore += targetValue * 2;
						src->SetMerged(true);
						tilesMoved = true;
					}
				}
			}
		}
	} else if (dir == TILE_MOVE_LEFT) {
		for (int col = 1; col < 4; col++) {
			for (int row = 0; row < 4; row++) {
				Tile *src = TileAt(row, col);
				if (src != NULL) {
					int targetValue = CheckAt(row + rowShift, col + colShift);
					if (targetValue == 0) {
						src->MoveBy(rowShift, colShift);
						tilesMoved = true;
					} else if (targetValue == src->Value() && !src->Merged()) {
						Tile *dst = TileAt(row + rowShift, col + colShift);
						dst->SetDeleted(true);
						src->MoveBy(rowShift, colShift);
						fScore += targetValue * 2;
						src->SetMerged(true);
						tilesMoved = true;
					}
				}
			}
		}
	}  else if (dir == TILE_MOVE_RIGHT) {
		for (int col = 2; col >= 0; col--) {
			for (int row = 0; row < 4; row++) {
				Tile *src = TileAt(row, col);
				if (src != NULL) {
					int targetValue = CheckAt(row + rowShift, col + colShift);
					if (targetValue == 0) {
						src->MoveBy(rowShift, colShift);
						tilesMoved = true;
					} else if (targetValue == src->Value() && !src->Merged()) {
						Tile *dst = TileAt(row + rowShift, col + colShift);
						dst->SetDeleted(true);
						src->MoveBy(rowShift, colShift);
						fScore += targetValue * 2;
						src->SetMerged(true);
						tilesMoved = true;
					}
				}
			}
		}
	}

	if (tilesMoved)
		fDropStep++;
	else {
		int32 i = 0;
		while (tileItem = (Tile*)fTileSet->ItemAt(i)) {
			if (tileItem->Deleted()) {
				fTileSet->RemoveItem(tileItem);
				delete tileItem;
			}
			i++;
		}
						
		for (int32 i = 0; tileItem = (Tile*)fTileSet->ItemAt(i); i++) {
			if (tileItem->Merged() && !tileItem->Deleted()) {
				tileItem->SetValue(tileItem->Value() * 2);
				tileItem->SetMerged(false);
			}
		}

		fDropStep = 0;
	}
		

	for (int32 i = 0; tileItem = (Tile*)fTileSet->ItemAt(i); i++) {
		if (tileItem->Value() == 2048 && gameStatus != GAME_CONT) {
			BAlert *winAlert = new BAlert("You win!", "Congratulations!\nYou reached the 2048 tile!\nDo you want to continue playing?", "Yes", "No");
			if (winAlert->Go() == 1)
				gameStatus = GAME_WIN;
			else
				gameStatus = GAME_CONT;
		}
	}
	
	if (!checkForMove()) {
		gameStatus = GAME_OVER;
	}

	return tilesMoved;	
}

bool
GameManager::NewTile(void)
{	
	if (!checkForMove()) {
		gameStatus = GAME_OVER;
		return false;
	}

	BList *emptyTiles = new BList();
	
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++) {
			if (TileAt(row, col) == NULL) {
				int val = (rand() % 100) <= 90 ? 2 : 4;
				Tile *tile = new Tile(row, col, val);
				emptyTiles->AddItem(tile);
			}
		}
	
	int randomIndex = rand() % emptyTiles->CountItems();
	Tile *randomTile = new Tile( (Tile*)emptyTiles->ItemAt(randomIndex) );
	
	Tile *tileItem;
	for (int32 i = 0; tileItem = (Tile*)emptyTiles->ItemAt(i); i++)
		delete tileItem;

	delete emptyTiles;
	
	fTileSet->AddItem(randomTile);

	if (!checkForMove()) {
		gameStatus = GAME_OVER;
		return false;
	}
	
	return true;
}

void
GameManager::Restart(void)
{	
	Tile *tileItem;
	for ( int32 i = 0; tileItem = (Tile*)fTileSet->ItemAt(i); i++ )
   		delete tileItem;

	fTileSet->MakeEmpty();
	
	gameStatus = GAME_PLAY;
	fScore = 0;
}
