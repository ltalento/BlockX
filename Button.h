#pragma once

#include <SDL.h>
#include <functional>

class Button
{
public:
	Button( SDL_Rect& rect, SDL_Texture* texture, SDL_Rect& clipMouseOut, SDL_Rect& clipMouseOver, std::function<void()>& action );
	virtual ~Button();

	void HandleEvent(SDL_Event * evt);
	void Draw(SDL_Renderer* renderer);

private:
	SDL_Rect rect_;
	SDL_Texture* texture_; 
	SDL_Rect clipMouseOut_;
	SDL_Rect clipMouseOver_;
	SDL_Rect* currClip_;
	std::function<void()> actionFunc_;
};

