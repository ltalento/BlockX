#pragma once

#include <functional>
#include <memory>

class GameResources;
class Button;
union SDL_Event;
struct SDL_Renderer;
struct SDL_Texture;

class MasterSoundButton
{
public:
	MasterSoundButton( int x, int y, GameResources* gameResources );
	~MasterSoundButton();

	void HandleEvent(SDL_Event * evt);
	void Draw(SDL_Renderer* renderer);

private:

	void OnBtn();
	
	int x_;
	int y_;
	SDL_Texture* texture_;
	std::unique_ptr<Button> btnOn_;
	std::unique_ptr<Button> btnOff_;
	std::function<void()> btnAction_;

	Button* currBtn_;
};

