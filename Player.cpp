#include "Player.h"


Player::Player(SDL_Surface* screen, int& score) : Item(screen), score(score)
{
	Reset();
};

Player::Player(int x, int y, SDL_Surface* screen, int& score): Item(x, y, screen), score(score)
{
	Reset();
}
;


void Player::Accelerate()
{
	if (speed < MAX_SPEED && y > 0 && y <= POSITION_PLAYER)
		speed += 200;
}
;

void Player::SlowDown()
{
	if (speed > 0)
		speed -= 200;
}
;

void Player::Move(double deltaTime) {
	if(!death)
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
		speed = 0;
		explode++;
		sprite = SDL_LoadBMP("image/explode1.bmp");
	}
	else if (explode < 60) {
		explode++;
	}
	else if (explode == 60) {
		speed = 0;
		explode++;
		sprite = SDL_LoadBMP("image/explode2.bmp");
	}
	else if (explode < 90) {
		explode++;
	}

	if(haltScore){
		haltScoreTime += deltaTime;
		// halt score if destory non-enemy car (red)
		if (haltScoreTime >= HALT_TIME) {
			haltScore = false;
			haltScoreTime = 0;
		}
	}
	
	if (distance> 1000) {
		distance = 0;

		if(!haltScore)
			score += 10;

		// lifes add 1 when get enough score
		if (score % 100 == 0)
			lifes++;

		boardOffset += 10 * boardOffsetSign;
		if (boardOffset < 140 || boardOffset > 180)
			boardOffsetSign *= -1;
	}
	DrawBoard();

	if (y <= -500 || y > SCREEN_HEIGHT + 500) {
		x = SCREEN_WIDTH / 2;
		y = -500;
		speed = 0;
		return;
	}

	moveAccumulate += speed * deltaTime;
	moveAccumulate_row += deltaTime * direction * ROW_SPEED;
	if (moveAccumulate > 1) {
		distance++;
		moveAccumulate -= 1;
	}
	if (moveAccumulate_row > 1) {
		x++;
		moveAccumulate_row -= 1;
	}
	else if (moveAccumulate_row < -1) {
		x--;
		moveAccumulate_row += 1;
	}
}
;

void Player::CheckDeath() {
	
	// if player is out of road
	if (x < screen->w / 2 - boardOffset || x > screen->w / 2 + boardOffset) {
		death = true;
	}

	if (death)
		lifes--;
}

void Player::DrawBoard() 
{
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		int offset = sin((double)(distance - i) / 5) * 10;
		int px = screen->w / 2 + offset - boardOffset;
		DrawPixel(screen, px, i, boardColor);
		px = screen->w / 2 + offset + boardOffset;
		DrawPixel(screen, px, i, boardColor);
	};
}
;

void Player::Reset() {
	explode = 100;
	death = false;
	haltScore = false;
	haltScoreTime = 0;
	x = SCREEN_WIDTH / 2;
	y = -500;
	speed = 0;
	direction = 0;
	sprite = SDL_LoadBMP("image/player.bmp");
}
;

// new game with invincible time, with unlimited lives
void Player::ResetLife() {
	lifes = NEWGAME_LIFE;
	unlimitLife = true;
	distance = 0;
	direction = 0;
}
;

// after invincible time
void Player::ResetNormalLife() {
	unlimitLife = false;
	lifes = NORMALMAX_LIFE;
}
;
