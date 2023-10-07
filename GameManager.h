/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

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
		BList*	TileSet(void) { return fTileSet; }
		bool	MoveTilesTo(int dir);
		Tile*	TileAt(int row, int col);
		int		CheckAt(int row, int col);
		int		Status(void) { return gameStatus; }
		void	SetStatus(int status) { gameStatus = status; }
		int		Score(void) { return fScore; }
		void	SetScore(int score) { fScore = score; }
		int		HighScore(void) { return fHighScore; }
		void	SetHighScore(int score) { fHighScore = score; }

	private:
		bool	checkForMove(void);

		BList 	*fTileSet;
		int		gameStatus;
		int		fScore;
		int		fHighScore;
		int		fDropStep;
};

#endif

