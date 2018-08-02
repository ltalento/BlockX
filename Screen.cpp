#include "Screen.h"

#include <SDL.h>

//------------------------------------------------
//Screen
Screen::Screen( ScreenManager* screenMngr )
	: screenMngr_( screenMngr )
{
}

Screen::~Screen()
{
}

//------------------------------------------------
//ScreenManager
ScreenManager::ScreenManager( ScreenManager* parent )
	:Screen( parent )
{
	nextScreen_ = -1;
}

void ScreenManager::AddScreen(int id, Screen* Screen)
{
	screenMap_[id] = Screen;	
}

void ScreenManager::Start(int screenId) 
{
	currScreen_ = screenMap_[screenId];
	currScreen_->Activate();
}

void ScreenManager::SetNextScreen(int screenId)
{
	nextScreen_ = screenId;
}

void ScreenManager::Activate()
{
	if (nullptr != currScreen_)
		currScreen_->Activate();
}

void ScreenManager::Suspend()
{
	if (nullptr != currScreen_)
		currScreen_->Suspend();
}

void ScreenManager::Check() 
{
	if (-1 != nextScreen_ )
	{
		if (nullptr != currScreen_)
			currScreen_->Suspend();
		
		currScreen_ = screenMap_[nextScreen_];

		currScreen_->Activate();

		nextScreen_ = -1;
	}

	if (nullptr != currScreen_)
		currScreen_->Check();
}

void ScreenManager::HandleEvent(SDL_Event* evt) 
{
	if (nullptr != currScreen_)
		currScreen_->HandleEvent( evt );
}

void ScreenManager::Update(double deltaTime)
{
	if (nullptr != currScreen_)
		currScreen_->Update(deltaTime);
}

void ScreenManager::Draw( SDL_Renderer* renderer)
{
	if (nullptr != currScreen_)
		currScreen_->Draw(renderer );
}

//------------------------------------------------

