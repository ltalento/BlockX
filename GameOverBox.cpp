#include "GameOverBox.h"
#include "GameResources.h"
#include "Button.h"
#include "NumberPrinter.h"

#include <SDL.h>
#include <string>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

const int GAME_OVER_BOX_WIDTH = 320;
const int GAME_OVER_BOX_HEIGHT = 240;

const int SCORE_YPOS = 138;

GameOverBox::GameOverBox( GameResources * gameResources, std::function<void(bool)>& onExit)
	:  texture_(nullptr), onExit_( onExit ), active_( false ), scaleX_(1.0), scaleY_(1.0), scaleTarget_( 1.0 ), state_(NOT_SET), exitBecauseRestart_( false ), totalScore_( 0 )
{
	texture_ = gameResources->GetTexture(GameTextures::GAME_OVER_BOX);

	const int GameOverBoxX = SCREEN_WIDTH / 2 - GAME_OVER_BOX_WIDTH / 2;
	const int GameOverBoxY = SCREEN_HEIGHT / 2 - GAME_OVER_BOX_HEIGHT / 2;
		
	//btn home
	{
		const int BTN_HOME_X = 33;
		const int BTN_HOME_Y = 184;

		int x = GameOverBoxX + BTN_HOME_X;
		int y = GameOverBoxY + BTN_HOME_Y;

		SDL_Rect btnHomeRect = { x, y, 40, 40 };
		SDL_Rect btnHomeClipMouseOut = { 0, 0, 40, 40 };
		SDL_Rect btnHomeClipMouseOver = { 40, 0, 40, 40 };

		SDL_Texture* btnHomeTexture = gameResources->GetTexture(GameTextures::BTN_HOME); 

		onHome_ = [&]() { OnHome(); };

		btnHome_.reset(new Button(btnHomeRect, btnHomeTexture, btnHomeClipMouseOut, btnHomeClipMouseOver, onHome_ ));
	}

	//btn restart
	{
		const int BTN_RESTART_X = 241;
		const int BTN_RESTART_Y = 184;

		int x = GameOverBoxX + BTN_RESTART_X;
		int y = GameOverBoxY + BTN_RESTART_Y;

		SDL_Rect btnRestartRect = { x, y, 40, 40 };
		SDL_Rect btnRestartClipMouseOut = { 0, 0, 40, 40 };
		SDL_Rect btnRestartClipMouseOver = { 40, 0, 40, 40 };

		SDL_Texture* btnRestartTexture = gameResources->GetTexture(GameTextures::BTN_RESTART);
		
		onRestart_ = [&]() { OnRestart(); };

		btnRestart_.reset( new Button(btnRestartRect, btnRestartTexture, btnRestartClipMouseOut, btnRestartClipMouseOver, onRestart_ ));
	}

	score_.reset(new NumberPrinter( GameOverBoxX + GAME_OVER_BOX_WIDTH/2, GameOverBoxY + SCORE_YPOS, gameResources, true));
}

GameOverBox::~GameOverBox()
{
}

void GameOverBox::HandleEvent(SDL_Event * evt)
{
	if (!active_)
		return;

	btnRestart_->HandleEvent(evt);
	btnHome_->HandleEvent(evt);
}

void GameOverBox::Update(double deltaTime)
{
	if (!active_)
		return;

	switch (state_)
	{
	case GameOverBoxStates::NOT_SET:
		break;
	case GameOverBoxStates::STARTING:
		scaleX_ += (scaleTarget_ - scaleX_) * 0.1;
		scaleY_ += (scaleTarget_ - scaleY_) * 0.1;

		if (std::abs(scaleTarget_ - scaleX_) < 0.02 || std::abs(scaleTarget_ - scaleY_) < 0.02)
		{
			scaleX_ = scaleTarget_;
			scaleY_ = scaleTarget_;
			state_ = GameOverBoxStates::SHOWING_SCORES;
		}
		break;
	case GameOverBoxStates::SHOWING_SCORES:
		{
			int len = static_cast<int>( std::to_string( score_->GetNumber() ).size() );

			score_->SetNumber(score_->GetNumber() + static_cast<int>( std::pow( 10, len-1 ) ) );

			if (totalScore_ <= score_->GetNumber())
			{
				score_->SetNumber(totalScore_);
				state_ = GameOverBoxStates::IDLING;
			}
		}
		break;
	case GameOverBoxStates::IDLING:
		break;
	case GameOverBoxStates::EXITING:
		scaleX_ += (scaleTarget_ - scaleX_) * 0.2;
		scaleY_ += (scaleTarget_ - scaleY_) * 0.2;

		if (std::abs(scaleTarget_ - scaleX_) < 0.02 || std::abs(scaleTarget_ - scaleY_) < 0.02)
		{
			scaleX_ = scaleTarget_;
			scaleY_ = scaleTarget_;
			state_ = GameOverBoxStates::NOT_SET;
			active_ = false;

			onExit_(exitBecauseRestart_);
		}
		break;
	}
}

void GameOverBox::Draw(SDL_Renderer * renderer)
{
	if (!active_)
		return;

	int width = static_cast<int>( GAME_OVER_BOX_WIDTH * scaleX_ );
	int height = static_cast<int>( GAME_OVER_BOX_HEIGHT * scaleY_ );

	int x = SCREEN_WIDTH / 2 - width / 2;
	int y = SCREEN_HEIGHT / 2 - height / 2;

	SDL_Rect rect = { x, y, width, height };
	SDL_RenderCopy(renderer, texture_, nullptr, &rect );


	score_->SetX( x + GAME_OVER_BOX_WIDTH/2 );
	score_->SetY( y + SCORE_YPOS );

	if (GameOverBoxStates::SHOWING_SCORES == state_ || GameOverBoxStates::IDLING == state_)
	{
		btnRestart_->Draw(renderer);
		btnHome_->Draw(renderer);		

		score_->Draw(renderer);
	}
}

void GameOverBox::Show( unsigned int score ) 
{
	active_ = true;

	state_ = GameOverBoxStates::STARTING;
	scaleX_ = 0.1;
	scaleY_ = 0.1;
	scaleTarget_ = 1;
	totalScore_ = score;	
	score_->SetNumber( 0 );
}

void GameOverBox::Close() 
{
	state_ = GameOverBoxStates::NOT_SET;
	active_ = false;	
}

void GameOverBox::OnHome() 
{
	scaleTarget_ = 0.1;
	exitBecauseRestart_ = false;
	state_ = GameOverBoxStates::EXITING;	
}

void GameOverBox::OnRestart() 
{
	scaleTarget_ = 0.1;
	exitBecauseRestart_ = true;
	state_ = GameOverBoxStates::EXITING;	
}
