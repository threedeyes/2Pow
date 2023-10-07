/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _TILE_H
#define _TILE_H

#define ANIMATION_STEPS 4

class Tile {
	public:
				Tile(int row, int col, int val);
				Tile(Tile* tile);
				~Tile() { };

		void	SetValue(int val) { value = val; }
		void	SetMerged(bool val) { merged = val; }
		void	SetDeleted(bool val);

		void	MoveTo(int row, int col);
		void	MoveBy(int row_dir, int col_dir);

		bool	Merged(void) { return merged; }
		bool	Deleted(void) { return deleted; }
		bool	Created(void) { return created; }

		int		Row(void) { return currentRow; }
		int		Col(void) { return currentCol; }

		int		PreviousRow(void) { return previousRow; }
		int		PreviousCol(void) { return previousCol; }

		int		Value(void) { return value; }
		int		Step(void) { return step; }
		void	DoStep(void);

	private:
		int 	value;
		int 	currentRow;
		int 	currentCol;
		int 	previousRow;
		int 	previousCol;

		int		step;				

		bool	merged;
		bool	deleted;
		bool	created;
};

#endif

