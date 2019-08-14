#ifndef _BOARD_VIEW_H
#define _BOARD_VIEW_H

#include <View.h>
#include <Bitmap.h>
#include <String.h>
#include <Font.h>

#include "GameManager.h"

#define COLOR_TILE	0
#define COLOR_TEXT	1
#define COLOR_FRAME 2

class BoardView : public BView
{
 public:
			 BoardView(BRect rect);
 			~BoardView();
 	void	SetGameManager(GameManager *game);
 	rgb_color ColorByValue(int value, int element = COLOR_TILE);
 	void	PaintBoard(void);
 	void 	MyDraw(void);
 private:
	virtual void MouseDown(BPoint p);
	virtual void Pulse(void);
	
	virtual void Draw(BRect rect);	
	virtual void AttachedToWindow();
	
	virtual void FrameResized(float width, float height);

	GameManager *gameManager;
	
	BView 		*offscreenView;
	BBitmap		*offscreenBitmap;	
};

#endif
