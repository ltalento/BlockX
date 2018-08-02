#include "MainMenuScreen.h"
#include "GameResources.h"
#include "Button.h"

#include <SDL.h>
#include <stdio.h>

MainMenuScreen::MainMenuScreen( ScreenManager* screenMngr, GameResources* gameResources )
 : Screen( screenMngr ), mainMenuTexture_( nullptr ), active_( false )
{	
	//main Menu texture
	{
		mainMenuTexture_ = gameResources->GetTexture(GameTextures::MAIN_MENU);
	}
	
	//btn play
	{
		SDL_Rect btnPlayRect = { 250, 250, 124, 64 };
		SDL_Rect btnPlayClipMouseOut = { 0, 0, 124, 64 };
		SDL_Rect btnPlayClipMouseOver = { 124, 0, 124, 64 };

		SDL_Texture* btnPlayTexture = gameResources->GetTexture(GameTextures::BTN_PLAY);

		btnAction_ = [&]() { OnPlay(); };

		btnPlay_ = std::unique_ptr<Button>(new Button(btnPlayRect, btnPlayTexture, btnPlayClipMouseOut, btnPlayClipMouseOver, btnAction_));
	}
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::Activate()
{
	active_ = true;
}

void MainMenuScreen::Suspend()
{
	active_ = false;
}

void MainMenuScreen::Check()
{
}

void MainMenuScreen::HandleEvent(SDL_Event * evt)
{
	if (!active_)
		return;

	btnPlay_->HandleEvent(evt);
}

void MainMenuScreen::Update(double deltaTime)
{
}

void MainMenuScreen::Draw( SDL_Renderer* renderer )
{
	if (!active_)
		return;

	SDL_RenderCopy( renderer, mainMenuTexture_, NULL, NULL );

	btnPlay_->Draw(renderer);
}

void MainMenuScreen::OnPlay()
{
	//printf( "MainMenuScreen::OnPlay()\n" );
	
	screenMngr_->SetNextScreen( GameMainScreens::GAME_SCREEN );
}
