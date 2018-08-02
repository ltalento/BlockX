#include "GameResources.h"

#include <SDL.h>
#include <stdio.h>

#include <string>

GameResources::GameResources()
{
}

GameResources::~GameResources()
{
	for (auto& it : textureMap_)
	{
		SDL_DestroyTexture(it.second);
	}
}

bool GameResources::Load( SDL_Renderer* renderer )
{
	textureMap_[GameTextures::MAIN_MENU] = LoadTexture( renderer, "Resources\\MainMenu.bmp" );
	textureMap_[GameTextures::MAIN_GAME_SCREEN] = LoadTexture(renderer, "Resources\\MainGame.bmp");
	textureMap_[GameTextures::BTN_PLAY] = LoadTexture(renderer, "Resources\\BtnPlay.bmp");
	textureMap_[GameTextures::BTN_MASTER_SOUND] = LoadTexture(renderer, "Resources\\BtnSound.bmp");
	textureMap_[GameTextures::BTN_HOME] = LoadTexture(renderer, "Resources\\BtnHome.bmp");
	textureMap_[GameTextures::BTN_RESTART] = LoadTexture(renderer, "Resources\\BtnRestart.bmp");
	textureMap_[GameTextures::NUMBERS] = LoadTexture(renderer, "Resources\\Numbers.bmp");
	textureMap_[GameTextures::GAME_OVER_BOX] = LoadTexture(renderer, "Resources\\GameOver.bmp");

	for( auto& it : textureMap_ )
	{
		if( nullptr == it.second )
			return false;
	}

	return true;
}

SDL_Texture* GameResources::GetTexture(GameTextures texture) 
{
	return textureMap_[texture];
}

SDL_Texture* GameResources::LoadTexture(SDL_Renderer* renderer, const char* path )
{
	SDL_Texture* texture = nullptr;

	SDL_Surface* surface = SDL_LoadBMP( path );

	if (nullptr == surface)
	{
		//printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
		printf("Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return nullptr;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (nullptr == texture )
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
	}

	SDL_FreeSurface(surface);

	return texture;
}