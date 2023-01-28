#pragma once

#include "drawFunctions.h"
#include <random>
#define ROW_SPEED speed / 2

// ±¬Õ¨¶¯»­
// hit´ÎÊý

class Item
{
public:
	int x = SCREEN_WIDTH / 2;
	int y = -500;
	SDL_Surface* sprite = nullptr;
	SDL_Surface* screen;

	int speed = 0;
	int direction = 0; // -1 left, 1 right, 0 only forward
	double moveAccumulate = 0;
	double moveAccumulate_row = 0;
	int explode = 3;

	Item(SDL_Surface* screen);
	Item(int x, int y, SDL_Surface* screen);
	virtual void Move(double deltaTime) = 0;
	void DrawItem(double deltaTime);
};

