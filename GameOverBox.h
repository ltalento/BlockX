#pragma once

#include <memory>
#include <functional>

union SDL_Event;
struct SDL_Renderer;
struct SDL_Texture;
class GameResources;
class Button;
class NumberPrinter;

class GameOverBox
{
public:
	GameOverBox( GameResources* gameResources, std::function<void(bool)>& onExit );
	~GameOverBox();

	void HandleEvent(SDL_Event* evt);
	void Update(double deltaTime);
	void Draw(SDL_Renderer* renderer);

	void Show( unsigned int score );
	void Close();

private:

	enum GameOverBoxStates
	{
		NOT_SET,
		STARTING,
		SHOWING_SCORES,
		IDLING,
		EXITING,
	};

	bool active_;
	SDL_Texture* texture_;
	std::function<void(bool)>& onExit_;
	double scaleX_;
	double scaleY_;
	double scaleTarget_;
	GameOverBoxStates state_;
	std::unique_ptr<Button> btnHome_;
	std::function<void()> onHome_;
	std::unique_ptr<Button> btnRestart_;
	std::function<void()> onRestart_;
	std::unique_ptr<NumberPrinter> score_;
	bool exitBecauseRestart_;
	unsigned int totalScore_;
	void OnHome();
	void OnRestart();
};

