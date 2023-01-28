#pragma once
#include "Player.h"
#define CAR_SPEED 200

class Car :
    public Item
{
public:
    bool active = false; // check whether this car is running in the game
    bool death;

    Player* player;

    Car(SDL_Surface* screen, Player* player);
    void Move(double deltaTime);
    void Reset();
    void ReLife();

    // collision with player
    void Collision();

    void CheckDeath();
};

