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



Tile::~Tile()
{
}

int
Tile::Value(void)
{
	return value;
}

bool
Tile::Merged(void)
{
	return merged;
}

bool
Tile::Created(void)
{
	return created;
}

bool
Tile::Deleted(void)
{
	return deleted;
}


int
Tile::Row(void)
{
	return currentRow;
}


int
Tile::Col(void)
{
	return currentCol;
}

int
Tile::PreviousRow(void)
{
	return previousRow;
}


int
Tile::PreviousCol(void)
{
	return previousCol;
}

int
Tile::Step(void)
{
	return step;
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
Tile::SetValue(int val)
{
	value = val;
}


void
Tile::SetMerged(bool val)
{
	merged = val;
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
