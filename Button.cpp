#include "Button.h"

//#include <SDL.h>


Button::Button( SDL_Rect& rect, SDL_Texture* texture, SDL_Rect& clipMouseOut, SDL_Rect& clipMouseOver, std::function<void()>& action )
	: rect_( rect ),
	texture_(texture ),
	clipMouseOut_(clipMouseOut ),
	clipMouseOver_(clipMouseOver ),
	actionFunc_( action )
{
	currClip_ = &clipMouseOut_;
}

Button::~Button()
{
}

void Button::HandleEvent(SDL_Event * evt)
{
	if( SDL_MOUSEMOTION == evt->type || SDL_MOUSEBUTTONUP  == evt->type )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = false;

		if( rect_.x <= x && rect_.x+rect_.w >= x && rect_.y <= y && rect_.y + rect_.h >= y) 
		{
			currClip_ = &clipMouseOver_;	

			inside = true;
		}
		else 
		{
			currClip_ = &clipMouseOut_;
		}

		if ( inside && SDL_MOUSEBUTTONUP == evt->type && SDL_BUTTON_LEFT == evt->button.button )
		{
			actionFunc_();
		}
	}
}

void Button::Draw(SDL_Renderer* renderer)
{
	SDL_Rect rect = rect_;

	rect.w = currClip_->w;
	rect.h = currClip_->h;

	SDL_RenderCopy( renderer, texture_, currClip_, &rect );
}