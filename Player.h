#pragma once
#include "Item.h"
#define MAX_SPEED 800
#define POSITION_PLAYER SCREEN_HEIGHT - 2 * sprite->h
#define HALT_TIME 5

class Player :
    public Item
{
public:
    int lifes = 1;	   
    int boardColor = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    int boardOffset = 160;
    int boardOffsetSign = -1;
    int boardDistance = 0;
    int distance = 0;
    int& score;
    bool unlimitLife;
    bool death;
    bool haltScore;
    double haltScoreTime;

    Player(SDL_Surface* screen, int &score);
    Player(int x, int y, SDL_Surface* screen, int& score);
    void Accelerate();
    void SlowDown();
    void Move(double deltaTime);
    void CheckDeath();
    void DrawBoard();

    // reset state
    void Reset();

    // reset unlimited life time life
    void ResetLife();

    // reset normal maximum life after unlimited life time
    void ResetNormalLife();
};

