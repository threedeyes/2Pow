/*
 * Copyright 2014-2023, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <math.h>

#include "Tile.h"
#include "BoardView.h"

BoardView::BoardView(BRect rect)
	:BView(rect,"BoardView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS|B_PULSE_NEEDED)
{
	fOffscreenView = new BView(Bounds(), "offscreenView", B_FOLLOW_NONE, 0);
	fOffscreenBitmap = new BBitmap(Bounds(), B_RGBA32, true);
	fOffscreenBitmap->AddChild(fOffscreenView);
	SetViewColor(B_TRANSPARENT_32_BIT);
	PaintBoard();
}

rgb_color
BoardView::ColorByValue(int value, int element)
{
	rgb_color black = {0,0,0,255};

	int colorIndex = (int)log2(value);	

	if (colorIndex < 0)
		return black;

	if (colorIndex > 11)
		colorIndex = (colorIndex - 12) % 3 + 4;

	rgb_color tileColors[] = {
		{0,0,0},			//	1
		{238, 228, 218},	//	2
		{237, 224, 200},	//	4
		{242, 177, 121},	//	8
		{245, 149, 99},		//	16
		{246, 124, 95},		//	32
		{246, 94, 59},		//	64
		{237, 207, 114},	//	128
		{237, 204, 97},		//	256
		{237, 200, 80},		//	512
		{237, 197, 63},		//	1024
		{245, 94, 99}		//	2048
	};
	
	rgb_color textColors[] = {
		{0,0,0},			//	1
		{80, 80, 80},		//	2
		{80, 80, 80},		//	4
		{249, 246, 242},	//	8
		{249, 246, 242},	//	16
		{249, 246, 242},	//	32
		{249, 246, 242},	//	64
		{249, 246, 242},	//	128
		{249, 246, 242},	//	256
		{249, 246, 242},	//	512
		{249, 246, 242},	//	1024
		{249, 246, 242}		//	2048
	};
	
	if (element == COLOR_TILE) {
		rgb_color color = tileColors[colorIndex];
		color.alpha = 255;
		return color;
	} else if (element == COLOR_TEXT) {
		rgb_color color = textColors[colorIndex];
		color.alpha = 255;
		return color;
	}
	return black;
}

void
BoardView::PaintBoard(void)
{
	Invalidate();
}

void
BoardView::MyDraw(void)
{
	Draw(Bounds());
}

void
BoardView::Draw(BRect rect)
{
 	fOffscreenBitmap->Lock();
	fOffscreenView->SetDrawingMode(B_OP_COPY);
	fOffscreenView->SetHighColor(250, 248, 239);
	fOffscreenView->FillRect(fOffscreenView->Bounds());
	
	float boardSize = 0.75 * MIN(fOffscreenView->Bounds().Width(), fOffscreenView->Bounds().Height());
	fOffscreenView->SetHighColor(187, 173, 160);
	
	float left = (fOffscreenView->Bounds().Width() - boardSize) / 2;
	float top = (fOffscreenView->Bounds().Height() - boardSize) / 2;
	float right = left + boardSize;
	float bottom = top + boardSize;
	
	fBoardRect.Set(left, top, right, bottom);
	BRect r1 = fBoardRect;
	r1.InsetBy(-10, -10);
	fOffscreenView->FillRoundRect(r1, 8, 8);

	float tileSize = boardSize / 4;

	BFont font;
	fOffscreenView->GetFont(&font);
	font.SetFace(B_BOLD_FACE);
	fOffscreenView->SetFont(&font);	
	
	fOffscreenView->SetDrawingMode(B_OP_BLEND);

	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++) {
			fOffscreenView->SetHighColor(204, 192, 179, 255);
			BRect tileRect(col*tileSize, row * tileSize,
			col*tileSize + (tileSize-1), row * tileSize + (tileSize-1));
			tileRect.InsetBy(6, 6);
			tileRect.OffsetBy(left, top);
			fOffscreenView->FillRoundRect(tileRect, 4, 4);
			fOffscreenView->SetHighColor(70, 70, 50, 200);
			fOffscreenView->StrokeRoundRect(tileRect, 4, 4);
		}

	if (gameManager->Status() == GAME_PLAY || gameManager->Status() == GAME_CONT) {
		BPoint mousePos;
		uint32 buttons;
		GetMouse(&mousePos, &buttons, false);
	
		fOffscreenView->SetHighColor(204, 192, 179, 150);
		if (mousePos.x < fBoardRect.left && mousePos.x > 1 &&
			mousePos.y > fBoardRect.top && mousePos.y < fBoardRect.bottom) {
				fOffscreenView->FillTriangle(BPoint(fBoardRect.left - 20, fBoardRect.top + 30),
					BPoint(fBoardRect.left - 20, fBoardRect.bottom - 30),
					BPoint(fBoardRect.left - 20 - (tileSize / 5), fOffscreenView->Bounds().Height() / 2));
		}
		if (mousePos.x > fBoardRect.right && mousePos.x < fOffscreenView->Bounds().right - 1 &&
			mousePos.y > fBoardRect.top && mousePos.y < fBoardRect.bottom) {
				fOffscreenView->FillTriangle(BPoint(fBoardRect.right + 20, fBoardRect.top + 30),
					BPoint(fBoardRect.right + 20, fBoardRect.bottom - 30),
					BPoint(fBoardRect.right + 20 + (tileSize / 5), fOffscreenView->Bounds().Height() / 2));
		}
		if (mousePos.x > fBoardRect.left && mousePos.x < fBoardRect.right &&
			mousePos.y > 1 && mousePos.y < fBoardRect.top) {
				fOffscreenView->FillTriangle(BPoint(fBoardRect.left + 30, fBoardRect.top - 20),
					BPoint(fBoardRect.right - 30, fBoardRect.top - 20),
					BPoint(fOffscreenView->Bounds().Width() / 2, fBoardRect.top - 20 - (tileSize / 5)));
		}
		if (mousePos.x > fBoardRect.left && mousePos.x < fBoardRect.right &&
			mousePos.y < fOffscreenView->Bounds().bottom - 1 && mousePos.y > fBoardRect.bottom) {
				fOffscreenView->FillTriangle(BPoint(fBoardRect.left + 30, fBoardRect.bottom + 20),
					BPoint(fBoardRect.right - 30, fBoardRect.bottom + 20),
					BPoint(fOffscreenView->Bounds().Width() / 2, fBoardRect.bottom + 20 + (tileSize / 5)));
		}
	}

	fOffscreenView->SetDrawingMode(B_OP_ALPHA);
	
	Tile *tile;
	for (int32 j = 0; j < 2; j++)
	for (int32 i = 0; tile = (Tile*)gameManager->TileSet()->ItemAt(i); i++) {
		if (j==0 && !tile->Deleted())
			continue;
		if (j==1 && tile->Deleted())
			continue;
		int row = tile->Row();
		int col = tile->Col();
	//		if (tile != NULL) {
				rgb_color color = ColorByValue(tile->Value(), COLOR_TILE);
				if (tile->Merged()) {
					rgb_color color2 = ColorByValue(tile->Value() * 2, COLOR_TILE);
					float pos = tile->Step() / (float)ANIMATION_STEPS;
					float red = color2.red + (color.red - color2.red) * pos;
					float green = color2.green + (color.green - color2.green) * pos;
					float blue = color2.blue + (color.blue - color2.blue) * pos;
					color.red = (int)red;
					color.green = (int)green;
					color.blue = (int)blue;
				}
				
				if (tile->Created()) {
					color.alpha = 255 - (255 * tile->Step() / (float)ANIMATION_STEPS);
				}
				
				fOffscreenView->SetHighColor(color);

				BRect tileRect(col * tileSize, row * tileSize,
					col*tileSize + (tileSize-1), row * tileSize + (tileSize-1));

				float rowDelta = (tile->PreviousRow() - tile->Row()) * tileSize * (tile->Step()) / (float)ANIMATION_STEPS;
				float colDelta = (tile->PreviousCol() - tile->Col()) * tileSize * (tile->Step()) / (float)ANIMATION_STEPS;
				
				tileRect.OffsetBy(left, top);
				tileRect.OffsetBy(colDelta, rowDelta);
				tileRect.InsetBy(6, 6);

				fOffscreenView->FillRoundRect(tileRect, 4, 4);
				
				float fontSize = tile->Value() <= 512 ? tileRect.Width() / 2.9 : tileRect.Width() / 3.7;

				BString textVal;
				textVal << tile->Value();

				//if ( tile->Deleted())
				//	fontSize *= tile->Step() / (float)(ANIMATION_STEPS);

				font.SetSize(fontSize);
				fOffscreenView->SetFont(&font);	
				BRect rectArray[1];
				font.GetBoundingBoxesAsGlyphs(textVal.String(), 1, B_SCREEN_METRIC, rectArray);
				float font_height = rectArray[0].Height();

								
				color = ColorByValue(tile->Value(), COLOR_TEXT);
				if (tile->Merged()) {					
					rgb_color color2 = ColorByValue(tile->Value() * 2, COLOR_TEXT);
					float pos = tile->Step() / (float)ANIMATION_STEPS;
					float red = color2.red + (color.red - color2.red) * pos;
					float green = color2.green + (color.green - color2.green) * pos;
					float blue = color2.blue + (color.blue - color2.blue) * pos;
					color.red = (int)red;
					color.green = (int)green;
					color.blue = (int)blue;
					if (pos < 0.5) {
						textVal = "";
						textVal << tile->Value() * 2;
					}
				}
				fOffscreenView->SetHighColor(color);
				
				float stringWidth = fOffscreenView->StringWidth(textVal.String());
				BPoint textPos = tileRect.LeftBottom();
				textPos.x += (tileRect.Width() - stringWidth) / 2;
				textPos.y -= (tileRect.Height() / 2) - (font_height) / 2;
			
				fOffscreenView->DrawString(textVal.String(), textPos);
				fOffscreenView->SetHighColor(70, 70, 50, 200);
				fOffscreenView->StrokeRoundRect(tileRect, 4, 4);
//			}
	}

	fOffscreenView->SetDrawingMode(B_OP_BLEND);

	font.SetSize(tileSize / 4);
	font.SetFace(B_REGULAR_FACE);
	fOffscreenView->SetFont(&font);	
	font_height height;
	font.GetHeight(&height);
	
	BString scoreText;
	scoreText << "SCORE: " << gameManager->Score();
	fOffscreenView->SetHighColor(0, 0, 0, 200);
	fOffscreenView->DrawString(scoreText.String(), BPoint(16, height.ascent));

	BString highScoreText;
	highScoreText << "BEST: " << gameManager->HighScore();
	fOffscreenView->DrawString(highScoreText.String(),
		BPoint(fOffscreenView->Bounds().Width() - 10 - fOffscreenView->StringWidth(highScoreText.String()),
		height.ascent));

	fOffscreenView->SetDrawingMode(B_OP_ALPHA);

	if (gameManager->Status() == GAME_OVER) {
		fOffscreenView->SetHighColor(0, 0, 0, 150);
		fOffscreenView->FillRect(Bounds());
		
		float fontSize = tileSize / 3;
		font.SetSize(fontSize);
		font.SetFace(B_BOLD_FACE);
		fOffscreenView->SetFont(&font);
		BString gameOverText("Game over");
		float stringWidth = fOffscreenView->StringWidth(gameOverText.String());
		fOffscreenView->SetHighColor(255, 255, 255, 200);
		BPoint textPos = fBoardRect.LeftBottom();
				textPos.x += (fBoardRect.Width() - stringWidth) / 2;
				textPos.y -= (fBoardRect.Height() / 2) - (fontSize / 2);		
		fOffscreenView->DrawString(gameOverText.String(), textPos);		
	}
	
	if (gameManager->Status() == GAME_WIN) {
		fOffscreenView->SetHighColor(0, 0, 0, 150);
		fOffscreenView->FillRect(Bounds());
		
		float fontSize = tileSize / 3;
		font.SetSize(fontSize);
		font.SetFace(B_BOLD_FACE);
		fOffscreenView->SetFont(&font);
		BString gameOverText("You win!");
		float stringWidth = fOffscreenView->StringWidth(gameOverText.String());
		fOffscreenView->SetHighColor(50, 250, 50, 190);
		BPoint textPos = fBoardRect.LeftBottom();
				textPos.x += (fBoardRect.Width() - stringWidth) / 2;
				textPos.y -= (fBoardRect.Height() / 2) - (fontSize / 2);		
		fOffscreenView->DrawString(gameOverText.String(), textPos);		
	}
	fOffscreenView->Sync();
	SetDrawingMode(B_OP_COPY);
	DrawBitmap(fOffscreenBitmap, rect, rect);
	fOffscreenBitmap->Unlock();
}

void
BoardView::SetGameManager(GameManager *game)
{
	gameManager = game;
}

void
BoardView::MouseDown(BPoint p)
{
	BMessage msg(B_KEY_DOWN);
	if (p.x < fBoardRect.left) {
		msg.AddInt32("key", 97);
		Window()->PostMessage(&msg);
	}
	if (p.x > fBoardRect.right) {
		msg.AddInt32("key", 99);
		Window()->PostMessage(&msg);
	}	
	if (p.y < fBoardRect.top) {
		msg.AddInt32("key", 87);
		Window()->PostMessage(&msg);
	}	
	if (p.y > fBoardRect.bottom) {
		msg.AddInt32("key", 98);
		Window()->PostMessage(&msg);
	}
}

void
BoardView::MouseMoved(BPoint p, uint32 transit,const BMessage *message)
{
	Invalidate();
}

void
BoardView::FrameResized(float width, float height)
{
	fOffscreenBitmap->RemoveChild(fOffscreenView);
	delete fOffscreenBitmap;
	
	fOffscreenView->ResizeTo(width, height);
	fOffscreenBitmap = new BBitmap(BRect(0, 0, width, height), B_RGBA32, true);
	fOffscreenBitmap->AddChild(fOffscreenView);	
	
	Invalidate();
}
