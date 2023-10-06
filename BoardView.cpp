#include <math.h>

#include "Tile.h"
#include "BoardView.h"

BoardView::BoardView(BRect rect)
	:BView(rect,"BoardView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS|B_PULSE_NEEDED)
{
	offscreenView = new BView(Bounds(), "offscreenView", B_FOLLOW_NONE, 0);
	offscreenBitmap = new BBitmap(Bounds(), B_RGBA32, true);
	offscreenBitmap->AddChild(offscreenView);
	SetViewColor(B_TRANSPARENT_32_BIT);
	PaintBoard();
}


BoardView::~BoardView()
{
}

rgb_color
BoardView::ColorByValue(int value, int element)
{
	int colorIndex = (int)log2(value);

	if (colorIndex < 0) {
		rgb_color black = {0,0,0,255};
		return black;
	}
	if (colorIndex > 11) {
		colorIndex = (colorIndex - 12) % 3 + 4;
	}

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
 	offscreenBitmap->Lock();
	offscreenView->SetDrawingMode(B_OP_COPY);
	offscreenView->SetHighColor(250, 248, 239);
	offscreenView->FillRect(offscreenView->Bounds());
	
	float boardSize = 0.75 * MIN(offscreenView->Bounds().Width(), offscreenView->Bounds().Height());
	offscreenView->SetHighColor(187, 173, 160);
	
	float left = offscreenView->Bounds().Width() / 2 - boardSize / 2;
	float top = 24 + (offscreenView->Bounds().Height()-24) / 2 - boardSize / 2;
	float right = left + boardSize;
	float bottom = top + boardSize;
	
	BRect boardRect(left, top, right, bottom);
	BRect r1 = boardRect;
	r1.InsetBy(-10, -10);
	offscreenView->FillRoundRect(r1, 8, 8);
	
	float tileSize = boardSize / 4;

	BFont font;
	offscreenView->GetFont(&font);
	font.SetFace(B_BOLD_FACE);
	offscreenView->SetFont(&font);	
	
	offscreenView->SetDrawingMode(B_OP_ALPHA);

	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++) {
			offscreenView->SetHighColor(204, 192, 179, 255);
			BRect tileRect(col*tileSize, row * tileSize,
			col*tileSize + (tileSize-1), row * tileSize + (tileSize-1));
			tileRect.InsetBy(6, 6);
			tileRect.OffsetBy(left, top);
			offscreenView->FillRoundRect(tileRect, 4, 4);
			offscreenView->SetHighColor(70, 70, 50, 200);
			offscreenView->StrokeRoundRect(tileRect, 4, 4);
		}
	
	//for (int row = 0; row < 4; row++)
	//	for (int col = 0; col < 4; col++) {
	//		Tile *tile = gameManager->TileAt(row, col);
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
				
				offscreenView->SetHighColor(color);

				BRect tileRect(col * tileSize, row * tileSize,
					col*tileSize + (tileSize-1), row * tileSize + (tileSize-1));

				float rowDelta = (tile->PreviousRow() - tile->Row()) * tileSize * (tile->Step()) / (float)ANIMATION_STEPS;
				float colDelta = (tile->PreviousCol() - tile->Col()) * tileSize * (tile->Step()) / (float)ANIMATION_STEPS;
				
				tileRect.OffsetBy(left, top);
				tileRect.OffsetBy(colDelta, rowDelta);
				tileRect.InsetBy(6, 6);

				offscreenView->FillRoundRect(tileRect, 4, 4);
				
				float fontSize = tile->Value() <= 512 ? tileRect.Width() / 2.9 : tileRect.Width() / 3.7;

				BString textVal;
				textVal << tile->Value();

				//if ( tile->Deleted())
				//	fontSize *= tile->Step() / (float)(ANIMATION_STEPS);

				font.SetSize(fontSize);
				offscreenView->SetFont(&font);	
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
				offscreenView->SetHighColor(color);
				
				float stringWidth = offscreenView->StringWidth(textVal.String());
				BPoint textPos = tileRect.LeftBottom();
				textPos.x += (tileRect.Width() - stringWidth) / 2;
				textPos.y -= (tileRect.Height() / 2) - (font_height) / 2;
			
				offscreenView->DrawString(textVal.String(), textPos);
				offscreenView->SetHighColor(70, 70, 50, 200);
				offscreenView->StrokeRoundRect(tileRect, 4, 4);
//			}
	}
	
	font.SetSize(tileSize / 5);
	font.SetFace(B_REGULAR_FACE);
	offscreenView->SetFont(&font);	
	font_height height;
	font.GetHeight(&height);
	
	BString scoreText;
	scoreText << "Score: " << gameManager->Score();
	offscreenView->SetHighColor(70, 70, 50, 200);
	offscreenView->DrawString(scoreText.String(), BPoint(10, 22 + height.ascent));

	BString highScoreText;
	highScoreText << "High Score: " << gameManager->HighScore();
	offscreenView->DrawString(highScoreText.String(),
		BPoint(offscreenView->Bounds().Width() - 10 - offscreenView->StringWidth(highScoreText.String()),
		22 + height.ascent));

	if (gameManager->Status() == GAME_OVER) {
		offscreenView->SetHighColor(0, 0, 0, 150);
		offscreenView->FillRect(Bounds());
		
		float fontSize = tileSize / 3;
		font.SetSize(fontSize);
		font.SetFace(B_BOLD_FACE);
		offscreenView->SetFont(&font);
		BString gameOverText("Game over");
		float stringWidth = offscreenView->StringWidth(gameOverText.String());
		offscreenView->SetHighColor(255, 255, 255, 200);
		BPoint textPos = boardRect.LeftBottom();
				textPos.x += (boardRect.Width() - stringWidth) / 2;
				textPos.y -= (boardRect.Height() / 2) - (fontSize / 2);		
		offscreenView->DrawString(gameOverText.String(), textPos);		
	}
	
	if (gameManager->Status() == GAME_WIN) {
		offscreenView->SetHighColor(0, 0, 0, 150);
		offscreenView->FillRect(Bounds());
		
		float fontSize = tileSize / 3;
		font.SetSize(fontSize);
		font.SetFace(B_BOLD_FACE);
		offscreenView->SetFont(&font);
		BString gameOverText("You win!");
		float stringWidth = offscreenView->StringWidth(gameOverText.String());
		offscreenView->SetHighColor(50, 250, 50, 190);
		BPoint textPos = boardRect.LeftBottom();
				textPos.x += (boardRect.Width() - stringWidth) / 2;
				textPos.y -= (boardRect.Height() / 2) - (fontSize / 2);		
		offscreenView->DrawString(gameOverText.String(), textPos);		
	}
	offscreenView->Sync();
	SetDrawingMode(B_OP_COPY);
	DrawBitmap(offscreenBitmap, rect, rect);
	offscreenBitmap->Unlock();
}


void
BoardView::AttachedToWindow()
{
	
}

void
BoardView::SetGameManager(GameManager *game)
{
	gameManager = game;
}


void
BoardView::MouseDown(BPoint p)
{
	
}

void
BoardView::Pulse(void)
{
}

void
BoardView::FrameResized(float width, float height)
{
	offscreenBitmap->RemoveChild(offscreenView);
	delete offscreenBitmap;
	
	offscreenView->ResizeTo(width, height);
	offscreenBitmap = new BBitmap(BRect(0, 0, width, height), B_RGBA32, true);
	offscreenBitmap->AddChild(offscreenView);	
	
	Invalidate();
}
