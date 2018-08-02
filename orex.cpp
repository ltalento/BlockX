
#include "GameResources.h"
#include "Screen.h"
#include "MainMenuScreen.h"
#include "MainGameScreen.h"
#include "MasterSoundButton.h"

#include <stdio.h>
#include <SDL.h>


//Screen dimension constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

void ShowSDLError( const char* msg )
{
	char buffer[1024]="";
	sprintf_s(buffer, sizeof(buffer),"%s%s\n", msg, SDL_GetError());
	SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Error", buffer, NULL);
}
		
int main(int argc, char* argv[]) 
{
	//SDL_SetMainReady();
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		ShowSDLError( "Could not Initialize SDL: " );
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("OREX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );

	if ( nullptr == window )
	{
		ShowSDLError( "Could not create window: " );
		SDL_Quit();
		return 1;
	}

	if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") )
	{
		printf("Warning: Linear texture filtering not enabled!");
	}
	
	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	
	if ( nullptr == renderer )
	{
		ShowSDLError( "Renderer could not be created! SDL Error: " );
		SDL_DestroyWindow( window );
		SDL_Quit();
		return 1;
	}


	GameResources gameResources;
	if( !gameResources.Load( renderer ) )
	{
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Error Loading game resources.\n", buffer, NULL);		
		SDL_DestroyRenderer( renderer );
		SDL_DestroyWindow( window );
		SDL_Quit();		
		return 1;
	} 

	MasterSoundButton masterSoundButton( 616, 440, &gameResources);

	ScreenManager sceneMngr;
	MainMenuScreen mainMenuScreen( &sceneMngr, &gameResources);
	MainGameScreen mainGameScreen( &sceneMngr, &gameResources);

	sceneMngr.AddScreen( 0, &mainMenuScreen );
	sceneMngr.AddScreen( 1, &mainGameScreen);
	
	sceneMngr.Start( 0 );

	auto startTime = SDL_GetTicks();
	bool running = true;
	while( running ) 
	{
		sceneMngr.Check();

		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			if (SDL_QUIT == event.type)
			{
				running = false;
				break;
			}

			masterSoundButton.HandleEvent( &event );
			sceneMngr.HandleEvent( &event );
		}
		
		double deltaTime = (SDL_GetTicks() - startTime ) / 1000.0;
		startTime = SDL_GetTicks();

		sceneMngr.Update( deltaTime );

		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear( renderer );

		sceneMngr.Draw(renderer);
		
		masterSoundButton.Draw(renderer);
		
		SDL_RenderPresent( renderer );

	}

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

