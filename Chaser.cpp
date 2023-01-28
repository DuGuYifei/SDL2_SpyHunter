#include "Chaser.h"

Chaser::Chaser(SDL_Surface* screen, Player* player) :Item(screen), player(player) {
	Reset();
};

void Chaser::Move(double deltaTime) {

	if (y < -500 || y > SCREEN_HEIGHT + sprite->h / 2) {
		Reset();
		return;
	}

	if (!death)
		CheckDeath();

	if (explode == 100 && death) {
		speed = 0;
		explode = 0;
		sprite = SDL_LoadBMP("image/explode0.bmp");
	}
	else if (explode < 30) {
		explode++;
	}
	else if (explode == 30) {
		explode++;
		sprite = SDL_LoadBMP("image/explode1.bmp");
	}
	else if (explode < 60) {
		explode++;
	}
	else if (explode == 60) {
		explode++;
		sprite = SDL_LoadBMP("image/explode2.bmp");
	}
	else if (explode < 90) {
		explode++;
	}
	else if (explode == 90) {
		Reset();
	}

	moveAccumulate += (speed - player->speed) * deltaTime;

	if (moveAccumulate > 1) {
		moveAccumulate -= 1;
		y--;
	}
	else if (moveAccumulate < -1) {
		moveAccumulate += 1;
		y++;
	}

	moveAccumulate_row += deltaTime * direction * ROW_SPEED * 10;
	if (moveAccumulate_row > 1) {
		x++;
		moveAccumulate_row -= 1;
	}
	else if (moveAccumulate_row < -1) {
		x--;
		moveAccumulate_row += 1;
	}
	
	Collision();

}

void Chaser::Reset() {
	direction = 0;
	sprite = SDL_LoadBMP("image/chaser.bmp");
	explode = 100;
	speed = 0;
	active = false;
	death = false;
	x = SCREEN_WIDTH / 2 + rand() % (player->boardOffset - 30) * (rand() / double(RAND_MAX) >= 0.5 ? 1 : -1);
	y = -500;
}

void Chaser::ReLife() {
	direction = 0;
	sprite = SDL_LoadBMP("image/chaser.bmp");
	explode = 100;
	speed = CHASER_SPEED;
	active = true;
	death = false;
	x = SCREEN_WIDTH / 2 + rand() % (player->boardOffset - 30) * (rand() / double(RAND_MAX) >= 0.5 ? 1 : -1);
	y = -sprite->h * 2;
}

// collision with player
void Chaser::Collision() {
	if (!player->death && !death) {
		// check player is collision in x-axis
		if (player->x - player->sprite->w / 2 <= x + sprite->w / 2 &&
			player->x + player->sprite->w / 2 >= x - sprite->w / 2)
		{

			// if collision from back or front
			if (player->y - player->sprite->h / 2 > y + sprite->h / 4 &&
				player->y - player->sprite->h / 2 <= y + sprite->h / 2 ||
				player->y + player->sprite->h / 2 < y - sprite->h / 4 &&
				player->y + player->sprite->h / 2 >= y - sprite->h / 2)
			{
				// if player speed is max then crash to bomb
				if (player->speed == MAX_SPEED) {
					player->death = true;
					death = true;
				}
				// else change chaser's track
				else {
					x -= player->sprite->w * (player->x > x ? 1 : -1);
				}
			}
			// collision from side
			else if (player->y - player->sprite->h / 2 <= y + sprite->h / 4 && player->y + player->sprite->h / 2 >= y - sprite->h / 4) {
				direction = player->direction;
			}
		}
	}
}

void Chaser::CheckDeath() {

	// if player is out of road
	if (x < screen->w / 2 - player->boardOffset || x > screen->w / 2 + player->boardOffset) {
		death = true;
	}
}