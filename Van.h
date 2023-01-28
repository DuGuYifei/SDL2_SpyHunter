#pragma once
#include "Player.h"

class Van :
    public Item
{
public:
    Player* player;

    Van(SDL_Surface* screen, Player* player);
    void NewLife();
    void Move(double deltaTime);
};

