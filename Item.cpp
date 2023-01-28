#include "item.h"

Item::Item(SDL_Surface* screen) :screen(screen) {};

Item::Item(int x, int y, SDL_Surface* screen) :x(x), y(y), screen(screen) {}


// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void Item::DrawItem(double deltaTime) {
	Move(deltaTime);
	DrawSurface(screen, this->sprite, this->x, this->y);
}

