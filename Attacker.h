#pragma once
#include "Player.h"
#define ATTACKER_SPEED 500

class Attacker :
    public Item
{
public:
    bool active = false; // check whether this car is running in the game
    bool death;

    Player* player;

    Attacker(SDL_Surface* screen, Player* player);
    void Move(double deltaTime);
    void Reset();
    void ReLife();

    // collision with player
    void Collision();

    void CheckDeath();
};

