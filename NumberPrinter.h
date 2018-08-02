#pragma once

#include <string>

class GameResources;
struct SDL_Renderer;
struct SDL_Texture;

class NumberPrinter
{
public:
	NumberPrinter( int x, int y, GameResources* gameResources, bool centred = false );
	~NumberPrinter();

	int GetX();
	int GetY();
	void SetX( int x );
	void SetY( int y );

	unsigned int GetNumber();
	void SetNumber( unsigned int number );

	void Draw(SDL_Renderer* renderer);

private:
	int x_;
	int y_;
	SDL_Texture* texture_;

	unsigned int currNumber_;
	std::string currNumberTxt_;
	bool centred_;
};

