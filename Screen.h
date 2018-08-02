#pragma once

#include <map>

class ScreenManager;
union SDL_Event;
struct SDL_Renderer;

class Screen
{
public:
	Screen(ScreenManager* screenMngr );
	virtual ~Screen();

	virtual void Activate() = 0;
	virtual void Suspend() = 0;

	virtual void Check() = 0;
	virtual void HandleEvent( SDL_Event* evt) = 0;
	virtual void Update(double deltaTime) = 0;
	virtual void Draw( SDL_Renderer* renderer) = 0;

protected:
	ScreenManager* screenMngr_;
};

enum GameMainScreens
{
	MENU_SCREEN,
	GAME_SCREEN
};

class ScreenManager : public Screen
{
public:
	ScreenManager( ScreenManager* parent = nullptr );

	void AddScreen(int id, Screen* screen );

	void Start(int ScreenId);
	void SetNextScreen(int screenId);
	
	virtual void Activate();
	virtual void Suspend();

	virtual void Check();
	virtual void HandleEvent(SDL_Event* evt);
	virtual void Update(double deltaTime);
	virtual void Draw( SDL_Renderer* renderer );

private:

	std::map<int, Screen*> screenMap_;
	Screen* currScreen_;
	int nextScreen_;
};