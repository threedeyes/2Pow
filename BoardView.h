/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _BOARD_VIEW_H
#define _BOARD_VIEW_H

#include <View.h>
#include <Bitmap.h>
#include <String.h>
#include <Font.h>
#include <Alert.h>

#include "GameManager.h"

#define COLOR_TILE	0
#define COLOR_TEXT	1
#define COLOR_FRAME 2

class BoardView : public BView
{
 public:
			 BoardView(BRect rect);
 			~BoardView() { };
 	void	SetGameManager(GameManager *game);
 	void	PaintBoard(void);
 	void 	MyDraw(void);
 	rgb_color ColorByValue(int value, int element = COLOR_TILE);

	virtual void MouseDown(BPoint p);
	virtual void MouseMoved(BPoint p, uint32 transit,const BMessage *message);
	virtual void Draw(BRect rect);	
	virtual void FrameResized(float width, float height);

 private:
	GameManager *gameManager;

	BRect		fBoardRect;
	BView 		*fOffscreenView;
	BBitmap		*fOffscreenBitmap;
};

#endif
