#ifndef _TILE_H
#define _TILE_H

#define ANIMATION_STEPS 4

class Tile {
	public:
				Tile(int row, int col, int val);
				Tile(Tile* tile);
				~Tile();
		
		void	SetValue(int val);
		void	SetMerged(bool val);
		void	SetDeleted(bool val);
		
		void	MoveTo(int row, int col);
		void	MoveBy(int row_dir, int col_dir);
		
		bool	Merged(void);
		bool	Deleted(void);
		bool	Created(void);
		
		int		Row(void);
		int		Col(void);

		int		PreviousRow(void);
		int		PreviousCol(void);
		
		int		Value(void);	
		int		Step(void);
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

