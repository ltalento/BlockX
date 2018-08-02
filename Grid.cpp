#include "Grid.h"

#include <SDL.h>

#include <cstdlib>
#include <ctime>
#include <set>
#include <queue>
#include <tuple>

int Random(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

Grid::Grid(int x, int y,
	int width, int height,
	int blockSize,
	std::function<void(std::vector<std::pair<int, int>>&)>& onBlockClick,
	std::function<void()>& onNoMoreMoves)
	: x_(x), y_(y), width_(width), height_(height),
	blockSize_(blockSize),
	onBlockClick_ ( onBlockClick ),
	onNoMoreMoves_ ( onNoMoreMoves ),
	isAnimating_(false)
{
	std::srand(static_cast<unsigned int>(std::time(0)));

	int coldIdx = 0;
	grid_.resize(width);
	for (int colIdx = 0; colIdx < width_; colIdx++ )
	{
		grid_[colIdx].resize(height_);
		for (int blockIdx = 0; blockIdx < height_; blockIdx++)
		{
			Block& block = grid_[colIdx][blockIdx];
			block.x_ = x_ + colIdx * blockSize_;
			block.y_ = y_ + blockIdx * blockSize_;

			if (width_ / 2 >= coldIdx)
			{
				block.color_ = BlockColor::GRAY;
			}
			else
			{
				block.color_ = static_cast<BlockColor>(Random(1, 5));
			}
		}
		coldIdx++;
	}
}

Grid::~Grid()
{
}

void Grid::HandleEvent(SDL_Event * evt)
{
	if (SDL_MOUSEMOTION == evt->type || SDL_MOUSEBUTTONUP == evt->type)
	{
		int x = 0, y = 0;
		SDL_GetMouseState(&x, &y);

		for (auto& col : grid_ )
		{
			for (auto& block : col)
			{
				block.selected_ = false;
			}
		}

		if (isAnimating_)
			return;

		int xGrid = (x - x_) / blockSize_;
		int yGrid = (y - y_) / blockSize_;

		if (0 <= xGrid && width_ > xGrid && 0 <= yGrid && height_ > yGrid)
		{
			bool click = false;

			BlockColor selectedColor = BlockColor::GRAY;
			if (BlockColor::GRAY != grid_[xGrid][yGrid].color_)
			{
				grid_[xGrid][yGrid].selected_ = true;

				if( SDL_MOUSEBUTTONUP == evt->type && SDL_BUTTON_LEFT == evt->button.button )
					click = true;
				
				//if (SDL_MOUSEBUTTONUP == evt->type && SDL_BUTTON_LEFT == evt->button.button)
				selectedColor = grid_[xGrid][yGrid].color_;
			}

			//check adjacent blocks
			if (BlockColor::GRAY != selectedColor)
			{
				std::vector<std::pair<int, int>> adjacentBlocks;

				GetAdjacentBlocks(adjacentBlocks, xGrid, yGrid, selectedColor);

				if( 1 < adjacentBlocks.size() )
				{
					int minAdjcentBlockX = width_ - 1;
					int maxAdjcentBlockX = 0;

					for (auto& block : adjacentBlocks)
					{
						std::tie(x, y) = block;

						if (click)
						{
							grid_[x][y].color_ = BlockColor::GRAY;
							grid_[x][y].selected_ = false;
						}
						else
							grid_[x][y].selected_ = true;

						minAdjcentBlockX = std::min(minAdjcentBlockX, x);
						maxAdjcentBlockX = std::max(maxAdjcentBlockX, x );
					}

					if (click)
					{
						std::vector<int> emptyCols;
						CheckVerticalStack(emptyCols, minAdjcentBlockX, maxAdjcentBlockX);
						CheckHorizontalStack(emptyCols);

						onBlockClick_( adjacentBlocks );

						if (!HasMoves())
						{
							onNoMoreMoves_();
						}
					}
				}
				//--------------------
			}
		}
	}
}

void Grid::Update(double deltaTime)
{
	isAnimating_ = false;

	if (1.0 < deltaTime)
		deltaTime = 0.1;

	if (0.0 >= deltaTime)
		deltaTime = 0.01;

	for (int x = 0; x < width_; x++ )
	{
		for (int y = 0; y < height_; y++ )
		{
			Block& block = grid_[x][y];

			int destX = x_ + x * blockSize_;
			int destY = y_ + y * blockSize_;

			if (BlockColor::GRAY == block.color_)
			{
				block.x_ = destX;
				block.y_ = destY;
				block.velX_ = 0;
				block.velY_ = 0;
				continue;
			}
		
			if (block.x_ == destX && block.y_ == destY)
			{
				block.velX_ = 0;
				block.velY_ = 0;
				continue;
			}
				
			isAnimating_ = true;

			int accX = 1000;
			int accY = 5000;
			
			block.velX_ += accX * deltaTime * ( 0 < (destX - block.x_ ) ? 1 : -1 );
			block.velY_ += accY * deltaTime * (0 < (destY - block.y_) ? 1 : -1);;

			const int halfBlockSize = blockSize_ / 2;
			const int maxSpeed = static_cast<int>( halfBlockSize * (1.0 / deltaTime) );
			
			if ( maxSpeed < std::abs( block.velX_ ) )
				block.velX_ = maxSpeed;

			if ( maxSpeed < std::abs( block.velY_ ) )
				block.velY_ = maxSpeed;

			block.x_ += block.velX_ * deltaTime + 0.5 * accX * deltaTime * deltaTime;
			block.y_ += block.velY_ * deltaTime + 0.5 * accY * deltaTime * deltaTime;

			if (std::abs(destX - block.x_) <= halfBlockSize )
			{
				block.x_ = destX;
				block.velX_ = 0;
			}

			if (std::abs(destY - block.y_) <= halfBlockSize)
			{
				block.y_ = destY;
				block.velY_ = 0;
			}
		}		
	}
}

void Grid::Draw(SDL_Renderer* renderer)
{
	for (auto& col : grid_)
	{
		for (auto& block : col)
		{
			if (BlockColor::GRAY == block.color_)
				continue;

			DrawBlock( renderer, block, blockSize_ );
		}
	}
}

void Grid::DrawBlock(SDL_Renderer * renderer, Block& block, int blockSize )
{
	switch (block.color_)
	{
	case ORANGE:
		if (block.selected_)
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x88, 0x00, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xBB, 0x33, 0xFF);
		break;
	case BLUE:
		if (block.selected_)
			SDL_SetRenderDrawColor(renderer, 0x00, 0x99, 0xCC, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 0x33, 0xB5, 0xE5, 0xFF);
		break;
	case GREEN:
		if (block.selected_)
			SDL_SetRenderDrawColor(renderer, 0x66, 0x99, 0x00, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 0x99, 0xCC, 0x00, 0xFF);
		break;
	case PURPLE:
		if (block.selected_)
			SDL_SetRenderDrawColor(renderer, 0x99, 0x33, 0xCC, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 0xAA, 0x66, 0xCC, 0xFF);
		break;
	case RED:
		if (block.selected_)
			SDL_SetRenderDrawColor(renderer, 0xCC, 0x00, 0x00, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x44, 0x44, 0xFF);
		break;
	case GRAY:
		SDL_SetRenderDrawColor(renderer, 0xD0, 0xD0, 0xD0, 0xFF);
		break;
	}
	
	
	int x = static_cast<int>(block.x_);
	int y = static_cast<int>(block.y_);
	SDL_Rect fillRect = { x , y , blockSize , blockSize };
	
	if (!block.selected_)
	{
		fillRect.x = x + 2;
		fillRect.y = y + 2;
		fillRect.w = blockSize - 4;
		fillRect.h = blockSize - 4;
	}

	SDL_RenderFillRect(renderer, &fillRect);
}

bool Grid::CheckGameOver()
{
	for (int y = 0; y < height_; y++)
	{
		if (BlockColor::GRAY != grid_[0][y].color_)
		{
			return true;
		}
	}
	return false;
}

void Grid::Grow()
{
	for (int x = 0; x < width_; x++)
	{
		for (int y = 0; y < height_; y++)
		{
			if (width_ - 1 == x)
			{
				grid_[x][y].color_ = BlockColor::GRAY;
				grid_[x][y].velX_ = 0;
				grid_[x][y].velY_ = 0;
			}
			else
			{
				grid_[x][y].color_ = grid_[x + 1][y].color_;
				grid_[x][y].x_ = grid_[x + 1][y].x_;
				grid_[x][y].y_ = grid_[x + 1][y].y_;

				grid_[x][y].velX_ = grid_[x + 1][y].velX_;
				grid_[x][y].velY_ = grid_[x + 1][y].velY_;
			}
		}
	}

	for (int y = 0; y < height_; y++)
	{
		grid_[width_ - 1][y].color_ = static_cast<BlockColor>(Random(1, 5));
		grid_[width_ - 1][y].x_ = x_ + width_ * blockSize_;
		grid_[width_ - 1][y].y_ = y_ + y * blockSize_;
		grid_[width_ - 1][y].velX_ = 0;
		grid_[width_ - 1][y].velY_ = 0;
	}
}

void Grid::GetAdjacentBlocks( std::vector<std::pair<int, int>>& adjacentBlocks, int startX, int startY, BlockColor color )
{
	if (BlockColor::GRAY == grid_[startX][startY].color_)
		return;

	std::queue<std::pair<int, int>> searchQueue;
	std::set<std::pair<int, int>> visitedList;

	searchQueue.push(std::make_pair(startX, startY));
	visitedList.insert(searchQueue.front());

	while (!searchQueue.empty())
	{
		std::pair<int, int> block = searchQueue.front();

		adjacentBlocks.push_back(block);

		int x, y;
		std::tie(x, y) = block;

		if (x + 1 < width_ && grid_[x + 1][y].color_ == color)
		{
			std::pair<int, int> block = std::make_pair(x + 1, y);

			if (visitedList.insert(block).second)
				searchQueue.push(block);
		}

		if (x - 1 >= 0 && grid_[x - 1][y].color_ == color)
		{
			std::pair<int, int> block = std::make_pair(x - 1, y);

			if (visitedList.insert(block).second)
				searchQueue.push(block);
		}

		if (y + 1 < height_ && grid_[x][y + 1].color_ == color)
		{
			std::pair<int, int> block = std::make_pair(x, y + 1);

			if (visitedList.insert(block).second)
				searchQueue.push(block);
		}

		if (y - 1 >= 0 && grid_[x][y - 1].color_ == color)
		{
			std::pair<int, int> block = std::make_pair(x, y - 1);

			if (visitedList.insert(block).second)
				searchQueue.push(block);
		}

		searchQueue.pop();
	}
}

bool Grid::HasMoves()
{
	bool hasMoves = false;

	for (int x = width_ - 1; 0 <= x && !hasMoves; x--)
	{
		for (int y = height_ - 1; 0 <= y; y--)
		{
			BlockColor refColor = grid_[x][y].color_;

			if (BlockColor::GRAY == refColor)
				continue;

			if (x + 1 < width_ && grid_[x + 1][y].color_ == refColor)
			{
				hasMoves = true;
				break;
			}

			if (x - 1 >= 0 && grid_[x - 1][y].color_ == refColor)
			{
				hasMoves = true;
				break;
			}

			if (y + 1 < height_ && grid_[x][y + 1].color_ == refColor)
			{
				hasMoves = true;
				break;
			}

			if (y - 1 >= 0 && grid_[x][y - 1].color_ == refColor)
			{
				hasMoves = true;
				break;
			}
		}
	}

	return hasMoves;
}

void Grid::CheckVerticalStack(std::vector<int>& emptyCols, int colFrom, int colTo)
{
	for (int x = colFrom; x <= colTo; x++)
	{
		bool isColEmpty = true;
		for (int y = height_ - 1; 0 <= y; y--)
		{
			if (BlockColor::GRAY == grid_[x][y].color_)
			{
				Block& grayBlock = grid_[x][y];
				for (int yy = y - 1; 0 <= yy; yy--)
				{
					if (BlockColor::GRAY != grid_[x][yy].color_)
					{
						Block& otherBlock = grid_[x][yy];

						grayBlock.color_ = otherBlock.color_;
						grayBlock.x_ = otherBlock.x_;
						grayBlock.y_ = otherBlock.y_;
						grayBlock.velX_ = 0;
						grayBlock.velY_ = blockSize_;

						otherBlock.color_ = BlockColor::GRAY;
						otherBlock.x_ = x_ + x * blockSize_;
						otherBlock.y_ = y_ + yy * blockSize_;
						otherBlock.velX_ = 0;
						otherBlock.velY_ = 0;

						isColEmpty = false;
						break;
					}
				}
			}
			else
			{
				isColEmpty = false;
			}
		}

		if (isColEmpty)
		{
			emptyCols.push_back(x);
		}
	}
}

void Grid::CheckHorizontalStack(std::vector<int>&emptyCols)
{
	for (int emptyCol : emptyCols)
	{
		for (int y = 0; y < height_; y++)
		{
			for (int x = emptyCol; 0 <= x; x--)
			{
				if (0 == x)
				{
					grid_[x][y].color_ = BlockColor::GRAY;
				}
				else
				{
					grid_[x][y].color_ = grid_[x - 1][y].color_;
					
					if( BlockColor::GRAY != grid_[x][y].color_ )
					{
						grid_[x][y].x_ = grid_[x - 1][y].x_;
						grid_[x][y].y_ = grid_[x - 1][y].y_;
						grid_[x][y].velX_ = grid_[x - 1][y].velX_;
						grid_[x][y].velY_ = grid_[x - 1][y].velY_;
					}
				}
			}
		}
	}
}

