#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include <List.h>
#include <Alert.h>

#include "Tile.h"

#define TILE_MOVE_LEFT		0
#define TILE_MOVE_RIGHT		1
#define TILE_MOVE_TOP		2
#define TILE_MOVE_BOTTOM	3

#define GAME_PLAY	0
#define GAME_OVER	1
#define GAME_WIN	2
#define GAME_CONT	3


class GameManager {
	public:
				GameManager();
				~GameManager();
		void	Restart(void);
		bool	NewTile(void);
		
		BList*	TileSet(void);
				
		bool	MoveTilesTo(int dir);
		
		Tile*	TileAt(int row, int col);
		
		int		CheckAt(int row, int col);
		
		int		Status(void);
		
		int		Score(void);
		
	private:
	
		bool	checkForMove(void);
	
		BList 	*fTileSet;
		int		gameStatus;
		
		int		fScore;
		
		int		fDropStep;
};

#endif

