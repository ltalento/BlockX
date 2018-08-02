#pragma once

#include "Screen.h"
#include <memory>
#include <functional>

class GameResources;
class Button;
struct SDL_Texture;

class MainMenuScreen : public Screen
{
public:
	MainMenuScreen( ScreenManager* screenMngr, GameResources* gameResources );
	~MainMenuScreen();

	void Activate();
	void Suspend();

	void Check();
	void HandleEvent(SDL_Event* evt);
	void Update(double timeStep);
	void Draw( SDL_Renderer* renderer );

private:

	void OnPlay();

	SDL_Texture* mainMenuTexture_;
	std::unique_ptr<Button> btnPlay_;
	std::function<void()> btnAction_;

	bool active_;
};

