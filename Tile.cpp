/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "Tile.h"

Tile::Tile(int row, int col, int val)
{
	step = ANIMATION_STEPS;
	currentRow = row;
	currentCol = col;
	previousRow = row;
	previousCol = col;
	value = val;
	deleted = false;
	merged = false;
	created = true;
}

Tile::Tile(Tile *tile)
{
	step = ANIMATION_STEPS;
	currentRow = tile->Row();
	currentCol = tile->Col();
	previousRow = tile->PreviousRow();
	previousCol = tile->PreviousCol();
	value = tile->Value();
	deleted = false;
	merged = false;
	created = true;
}

void
Tile::DoStep(void)
{
	if (step <= 0) {
		created = false;
		return;
	}
	step --;
}

void
Tile::SetDeleted(bool val)
{
	deleted = val;
	if (deleted) {
		previousRow = currentRow;
		previousCol = currentCol;	
		step = ANIMATION_STEPS;
	}
}

void
Tile::MoveTo(int row, int col)
{
	previousRow = currentRow;
	previousCol = currentCol;	
	currentRow = row;
	currentCol = col;
	step = ANIMATION_STEPS;
}

void
Tile::MoveBy(int row_dir, int col_dir)
{
	previousRow = currentRow;
	previousCol = currentCol;	
	currentRow += row_dir;
	currentCol += col_dir;
	step = ANIMATION_STEPS;
}
