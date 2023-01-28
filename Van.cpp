#include "Van.h"


Van::Van(SDL_Surface* screen, Player* player) : Item(screen), player(player) { 
	sprite = SDL_LoadBMP("image/van.bmp");
};

void Van::NewLife() {
	player->Reset();
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT + 50;
	speed = MAX_SPEED + 100;
}

void Van::Move(double deltaTime) {
	if (player->explode == 90 && player->lifes > 0){
		NewLife();
	}

	if (y < -500 || y > SCREEN_HEIGHT + 500) {
		x = -500;
		y = -500;
		speed = 0;
		return;
	}
	moveAccumulate += speed * deltaTime;

	if (moveAccumulate > 1) {
		moveAccumulate -= 1;
		y--;
	}

	if (y > POSITION_PLAYER) {
		player->speed = 0;
		player->x = x;
		player->y = y;
	}
}