#pragma once

#include "Screen.h"
#include "Grid.h"

#include <memory>
#include <functional>

class GameResources;
class Button;
struct SDL_Texture;
class NumberPrinter;
class GameOverBox;

enum GameState
{
	NOT_SET,
	PLAY,
	GAME_OVER
};

class MainGameScreen : public Screen
{
public:
	MainGameScreen( ScreenManager* screenMngr, GameResources* gameResources );
	~MainGameScreen();

	void Activate();
	void Suspend();

	void Check();
	void HandleEvent(SDL_Event* evt);
	void Update(double timeStep);
	void Draw( SDL_Renderer* renderer );

private:

	void OnHome();
	void OnBlockClick( std::vector<std::pair<int, int>>& blocks );
	void OnNoMoreMoves();

	void OnGameOverExit(bool reset);
	void StartGame();

	SDL_Texture* mainGameTexture_;
	std::unique_ptr<Button> btnHome_;
	std::function<void()> onHome_;

	std::unique_ptr<Grid> grid_;

	bool active_;

	double pushTimeCounter_;

	std::unique_ptr<NumberPrinter> score_;
	std::unique_ptr<NumberPrinter> pushCounter_;

	std::function<void(std::vector<std::pair<int, int>>&)> onBlockClick_;
	std::function<void()> onNoMoreMoves_;
	std::function<void(bool)> onGameOverExit_;

	GameState gameState_;
	
	std::unique_ptr<GameOverBox> gameOverBox_;
};

