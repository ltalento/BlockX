#pragma once

#include <vector>
#include <functional>

class ScreenManager;
union SDL_Event;
struct SDL_Renderer;

enum BlockColor
{
	GRAY,
	ORANGE,
	BLUE,
	GREEN,
	PURPLE,
	RED
};

struct Block
{
	BlockColor color_;
	double x_;
	double y_;
	double velX_;
	double velY_;

	bool selected_;

	Block()
		: selected_(false), color_(GRAY), x_(0), y_(0), velX_(0), velY_(0)
	{}
};

class Grid
{
	
public:
	Grid(int x, int y,
		int width, int height,
		int blockSize,
		std::function<void(std::vector<std::pair<int, int>>&)>& onBlockClick,
		std::function<void()>& onNoMoreMoves);

	~Grid();

	void HandleEvent(SDL_Event* evt);
	void Update(double deltaTime);
	void Draw(SDL_Renderer* renderer);
	bool CheckGameOver();
	void Grow();

private:

	static void DrawBlock(SDL_Renderer* renderer, Block& block, int blockSize );
	void GetAdjacentBlocks(std::vector<std::pair<int, int>>& adjacentBlocks, int startX, int startY, BlockColor color);
	bool HasMoves();
	void CheckVerticalStack(std::vector<int>& emptyCols, int colFrom, int colTo);
	void CheckHorizontalStack(std::vector<int>& emptyCols);

	int x_;
	int y_;
	int width_;
	int height_;
	int blockSize_;
	bool isAnimating_;
	std::vector<std::vector<Block>> grid_;
	std::function<void(std::vector<std::pair<int, int>>&)>& onBlockClick_;
	std::function<void()>& onNoMoreMoves_;
};

