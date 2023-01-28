#pragma once
#include "Player.h"
#define CHASER_SPEED 300

class Chaser :
    public Item
{
public:
    bool active = false; // check whether this car is running in the game
    bool death;

    Player* player;

    Chaser(SDL_Surface* screen, Player* player);
    void Move(double deltaTime);
    void Reset();
    void ReLife();

    // collision with player
    void Collision();

    void CheckDeath();
};

