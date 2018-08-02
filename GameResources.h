#pragma once

#include <map>

struct SDL_Texture;
struct SDL_Renderer;

enum GameTextures
{
	MAIN_MENU,
	MAIN_GAME_SCREEN,
	BTN_PLAY,
	BTN_MASTER_SOUND,
	BTN_HOME,
	BTN_RESTART,
	NUMBERS,
	GAME_OVER_BOX
};

class GameResources
{
public:
	GameResources();
	~GameResources();

	bool Load( SDL_Renderer* renderer );
	
	SDL_Texture* GetTexture( GameTextures texture );

private:

	SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path );

	std::map<GameTextures,SDL_Texture*> textureMap_;

};

