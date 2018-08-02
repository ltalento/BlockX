#include "NumberPrinter.h"
#include "GameResources.h"

#include <SDL.h>

const int NUMBER_FONT_WIDTH = 16;
const int NUMBER_FONT_HEIGHT = 23;

NumberPrinter::NumberPrinter( int x, int y, GameResources* gameResources, bool centred )
	:x_(x), y_(y), currNumber_( 0 ), centred_(centred)
{
	texture_ = gameResources->GetTexture( GameTextures::NUMBERS );

	SetNumber(currNumber_);
}

NumberPrinter::~NumberPrinter()
{
}

int NumberPrinter::GetX()
{
	return x_;
}

int NumberPrinter::GetY()
{
	return y_;
}

void NumberPrinter::SetX( int x )
{
	x_ = x;
}

void NumberPrinter::SetY( int y )
{
	y_ = y;
}

void NumberPrinter::SetNumber( unsigned int number )
{
	currNumber_ = number;
	currNumberTxt_ = std::to_string(currNumber_);
}

unsigned int NumberPrinter::GetNumber()
{
	return currNumber_;
}

void NumberPrinter::Draw(SDL_Renderer* renderer) 
{
	int len = static_cast<int>( currNumberTxt_.size() );

	for (int i = 0; i < len; i++)
	{
		char ch = currNumberTxt_[i];
		ch -= '0';

		if (9 < ch || 0 > ch)
			ch = 10;
		
		int x = x_ + i*NUMBER_FONT_WIDTH;
		if (centred_)
			x -= len*NUMBER_FONT_WIDTH / 2;

		SDL_Rect rect = { x, y_, NUMBER_FONT_WIDTH, 23 };

		SDL_Rect numberRect = { 0, ch*NUMBER_FONT_HEIGHT, NUMBER_FONT_WIDTH, NUMBER_FONT_HEIGHT };

		SDL_RenderCopy(renderer, texture_, &numberRect, &rect);
	}
}