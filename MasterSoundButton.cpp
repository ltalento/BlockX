#include "MasterSoundButton.h"
#include "GameResources.h"
#include "Button.h"

#include <SDL.h>
#include <stdio.h>

MasterSoundButton::MasterSoundButton( int x, int y, GameResources* gameResources )
	: x_( x ), y_( y ), currBtn_( nullptr )
{
	texture_ = gameResources->GetTexture(GameTextures::BTN_MASTER_SOUND);

	//btn on
	{
		SDL_Rect btnOnRect = { x, y, 24, 40 };
		SDL_Rect btnOnClipMouseOut = { 0, 0, 24, 40 };
		SDL_Rect btnOnClipMouseOver = { 24, 0, 24, 40 };

		btnAction_ = [&]() { OnBtn(); };

		btnOn_ = std::unique_ptr<Button>(new Button(btnOnRect, texture_, btnOnClipMouseOut, btnOnClipMouseOver, btnAction_));
	}
	
	//btn off
	{
		SDL_Rect btnOffRect = { x, y, 24, 40 };
		SDL_Rect btnOffClipMouseOut = { 48, 0, 24, 40 };
		SDL_Rect btnOffClipMouseOver = { 72, 0, 24, 40 };

		btnAction_ = [&]() { OnBtn(); };

		btnOff_ = std::unique_ptr<Button>(new Button(btnOffRect, texture_, btnOffClipMouseOut, btnOffClipMouseOver, btnAction_));
	}

	currBtn_ = btnOn_.get();
}

MasterSoundButton::~MasterSoundButton()
{
}

void MasterSoundButton::HandleEvent(SDL_Event * evt) 
{
	currBtn_->HandleEvent( evt );
}

void MasterSoundButton::Draw(SDL_Renderer* renderer) 
{
	currBtn_->Draw( renderer );
}

void MasterSoundButton::OnBtn()
{
	printf("MasterSoundButton::OnBtn()\n");

	if (currBtn_ == btnOn_.get())
	{
		currBtn_ = btnOff_.get();
	}
	else if (currBtn_ == btnOff_.get())
	{
		currBtn_ = btnOn_.get();
	}
}