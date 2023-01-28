#pragma once
#include "Van.h"
#include "Car.h"
#include "Chaser.h"
#include "Attacker.h"

#define CAR_NUM 2
#define CHASER_NUM 5
#define ATTACKER_NUM 5
#define UNLIMIT_LIFE_TIME 9.9
#define MEMORY_SIZE 5
#define RANK_FILE "rank.txt"

struct RankItem {
	int score;
	int time;
	RankItem(int score, int time) :score(score), time(time) {};

	// rank as higher score then higher time
	bool operator < (const RankItem& ri) const {
		return score == ri.score ? time < ri.time : score < ri.score;
	}
	// rank by higher time them higher score
	bool operator > (const RankItem& ri) const {
		return time == ri.time ? score > ri.score : time > ri.time;
	}
};

class StartGame
{
public:
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Renderer* renderer;
	SDL_Event event;

	int score, carScore; // every 20 score, will born a car
	int t1, t2, frames;
	bool pause, quit, pauseDown, playing;
	double delta, worldTime, fpsTimer, fps, distance, unlimitLifeTime;
	char text[128];

	int rankNum = 0;
	RankItem* rank;

	Player* player;
	Van* van;

	// Object pool
	Car* cars[CAR_NUM];
	// perioldical born enemy
	double prevChaserBornTime;
	Chaser* chasers[CHASER_NUM];
	double prevAttackerBornTime;
	Attacker* attackers[ATTACKER_NUM];

	// black
	int czarny;
	// green
	int zielony;
	// red
	int czerwony;
	// blue
	int niebieski;

	StartGame(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer);

	void NewGame();
	void InGame();
	void NPC_NotCrash();
	void WriteScore(int newScore, int newTime);
	void ReadScore();
	void SortRank(bool isSortByTime);
	RankItem GetIntFromStr();
	
};

