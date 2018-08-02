#include "MainGameScreen.h"
#include "GameResources.h"
#include "Button.h"
#include "NumberPrinter.h"
#include "GameOverBox.h"

#include <SDL.h>
#include <stdio.h>
#include <cmath>


const int PUSH_TIME = 4;

MainGameScreen::MainGameScreen( ScreenManager* screenMngr, GameResources* gameResources )
 : Screen( screenMngr ), mainGameTexture_( nullptr ),  active_( false ), pushTimeCounter_( 0.0 ), gameState_( GameState::NOT_SET )
{	
	//main Menu texture
	{
		mainGameTexture_ = gameResources->GetTexture(GameTextures::MAIN_GAME_SCREEN);
	}
	
	//btn home
	{
		SDL_Rect btnHomeRect = { 0, 480-40, 40, 40 };
		SDL_Rect btnHomeClipMouseOut = { 0, 0, 40, 40 };
		SDL_Rect btnHomeClipMouseOver = { 40, 0, 40, 40 };

		SDL_Texture* btnHomeTexture = gameResources->GetTexture(GameTextures::BTN_HOME);

		onHome_ = [&]() { OnHome(); };

		btnHome_.reset( new Button(btnHomeRect, btnHomeTexture, btnHomeClipMouseOut, btnHomeClipMouseOver, onHome_) );
	}

	score_.reset( new NumberPrinter(127, 10, gameResources ) );
	pushCounter_.reset(new NumberPrinter(586, 10, gameResources));

	onBlockClick_ = [&](std::vector<std::pair<int, int>>& blocks)
	{
		OnBlockClick(blocks);
	};

	onNoMoreMoves_ = [&]() { OnNoMoreMoves(); };
	
	onGameOverExit_ = [&]( bool reset ) { OnGameOverExit( reset ); };

	gameOverBox_.reset(new GameOverBox( gameResources, onGameOverExit_));
}

MainGameScreen::~MainGameScreen()
{
}

void MainGameScreen::Activate()
{
	active_ = true;

	StartGame();
}

void MainGameScreen::Suspend()
{
	active_ = false;
}

void MainGameScreen::Check()
{
}

void MainGameScreen::HandleEvent(SDL_Event * evt)
{
	if (!active_)
		return;	

	btnHome_->HandleEvent(evt);

	switch (gameState_)
	{
	case NOT_SET:
		break;
	case PLAY:
		grid_->HandleEvent(evt);
		break;
	case GAME_OVER:
		gameOverBox_->HandleEvent(evt);
		break;
	default:
		break;
	}

}

void MainGameScreen::Update(double deltaTime)
{
	switch (gameState_)
	{
	case NOT_SET:
		break;
	case PLAY:
		pushTimeCounter_ -= deltaTime;
		pushCounter_->SetNumber(static_cast<int>(std::floor(pushTimeCounter_)));

		if ( 0 > pushTimeCounter_)
		{
			pushTimeCounter_ = PUSH_TIME;
			pushCounter_->SetNumber(static_cast<int>(std::floor(pushTimeCounter_)));

			if (grid_->CheckGameOver())
			{
				//printf("GAME OVER\n");

				//transite to GAME OVER
				gameOverBox_->Show(score_->GetNumber());
				gameState_ = GameState::GAME_OVER;
				return;
			}

			grid_->Grow();
		}

		grid_->Update(deltaTime);
		break;
	case GAME_OVER:
		gameOverBox_->Update(deltaTime);
		break;
	default:
		break;
	}
}

void MainGameScreen::Draw( SDL_Renderer* renderer )
{
	if (!active_)
		return;

	SDL_RenderCopy( renderer, mainGameTexture_, NULL, NULL );

	btnHome_->Draw(renderer);
	
	grid_->Draw(renderer);

	score_->Draw(renderer);
	pushCounter_->Draw(renderer);

	gameOverBox_->Draw(renderer);
}

void MainGameScreen::OnHome()
{
	screenMngr_->SetNextScreen( GameMainScreens::MENU_SCREEN );
}

void MainGameScreen::OnBlockClick(std::vector<std::pair<int, int>>& blocks)
{
	score_->SetNumber( score_->GetNumber() + blocks.size() * 10 );

	//where is my particle system???
}

void MainGameScreen::OnNoMoreMoves()
{
	//printf("NO MORE MOVES\n");
}

void MainGameScreen::OnGameOverExit(bool reset) 
{
	if (reset)
	{
		StartGame();
		return;
	}

	OnHome();
}

void MainGameScreen::StartGame()
{
	gameState_ = GameState::PLAY;

	grid_.reset( new Grid(32, 60, 16, 10, 38, onBlockClick_, onNoMoreMoves_ ));
	score_->SetNumber( 0 );

	pushTimeCounter_ = PUSH_TIME;
	pushCounter_->SetNumber(static_cast<int>(std::floor(pushTimeCounter_)));
}
