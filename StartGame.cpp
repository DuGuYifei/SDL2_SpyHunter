#include "StartGame.h"
#include <time.h>

StartGame::StartGame(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer) :
	screen(screen), charset(charset), scrtex(scrtex), renderer(renderer)
{
	t1 = 0;
	score = 0;
	// black
	czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	// green
	zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	// red
	czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	// blue
	niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	srand((int)time(0));

	player = new Player(screen, score);
	van = new Van(screen, player);
	for (int i = 0; i < CAR_NUM; i++)
		cars[i] = new Car(screen, player);

	for (int i = 0; i < CHASER_NUM; i++)
		chasers[i] = new Chaser(screen, player);

	for (int i = 0; i < ATTACKER_NUM; i++)
		attackers[i] = new Attacker(screen, player);

	ReadScore();
};

void StartGame::InGame() {

	quit = false;
	pause = false;
	pauseDown = false;
	playing = false;
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	worldTime = 0;
	unlimitLifeTime = UNLIMIT_LIFE_TIME;
	
	while (!quit) {
		t2 = SDL_GetTicks();

		// here t2-t1 is the time in milliseconds since
		// the last screen was drawn
		// delta is the same time in seconds
		delta = (t2 - t1) * 0.001;
;
		t1 = t2;

		if (playing) {
			worldTime += delta;
		}
		fpsTimer += delta;

		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		// background
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);

		SDL_FillRect(screen, NULL, czarny);

		// call out items from object pool if satisfied conditions
		// 1. car (which will stop player's score if been destroyed)
		if (playing) {
			if (player->lifes == 0 && player->explode >= 90) {
				playing = false;
			}

			if (score - carScore >= 20) {
				for (auto& i : cars) {
					if (!i->active) {
						i->ReLife();
						carScore = score;
						break;
					}
				}
			}

			// 2. chaser (enemy that player can chase)
			if (worldTime > prevChaserBornTime + 2) {
				for (auto& i : chasers) {
					if (!i->active) {
						prevChaserBornTime = worldTime;
						i->ReLife();
						break;
					}
				}
			}

			// attacker (enemy can atack player)
			if (worldTime > prevAttackerBornTime + 2.5) {
				for (auto& i : attackers) {
					if (!i->active) {
						prevAttackerBornTime = worldTime;
						i->ReLife();
						break;
					}
				}
			}

			// update all objects
			// let npc not crash with each other
			NPC_NotCrash();

			// In DrawItem: 
			// 1. change x,y 
			// 2. check whether crash on the road board and play explode animation
			// 3. check collision with player according each mechanism
			// 4. update sdl_surface
			for (auto& i : cars) {
				if (i->active)
					i->DrawItem(delta);
			}

			for (auto& i : chasers) {
				if (i->active)
					i->DrawItem(delta);
			}

			for (auto& i : attackers) {
				if (i->active)
					i->DrawItem(delta);
			}

			van->DrawItem(delta);
			player->DrawItem(delta);

			// score lifes
			if (player->lifes < 100) {
				sprintf(text, "score: %d      lifes: %d", score, player->lifes);
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 20, text, charset);
			}
			else {
				sprintf(text, "score: %d      lifes: ???", score);
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 20, text, charset);
			}

			// invincible time
			if (worldTime < unlimitLifeTime) {
				sprintf(text, "invincible time: %.1f", unlimitLifeTime - worldTime);
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 30, text, charset);
			}
			else {
				unlimitLifeTime = 0;
				if (player->unlimitLife) {
					player->ResetNormalLife();
				}
			}
		}
		else {
			sprintf(text, "p - sort by score");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 100, text, charset);
			sprintf(text, "t - sort by time");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 110, text, charset);

			sprintf(text, "Rank   Score   Time");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 140, text, charset);

			for (int i = 0; i < rankNum; i++) {
				sprintf(text, "No.%d   %d    %d", i + 1, rank[i].score, rank[i].time);
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 150 + i * 10, text, charset);
			}
		}

		// time
		sprintf(text, "elapsed time = %.1f s  %.lf frames / s", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

		if (player->lifes == 0) {
			sprintf(text, "GAME OVER");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 20, text, charset);
		}

		// right bottom intro
		sprintf(text, "n - new game");
		DrawString(screen, screen->w - 15 * 8, screen->h - 11 * 9, text, charset);
		sprintf(text, "Esc - exit");
		DrawString(screen, screen->w - 15 * 8, screen->h - 10 * 9, text, charset);
		sprintf(text, "p - pause");
		DrawString(screen, screen->w - 15 * 8, screen->h - 9 * 9, text, charset);
		sprintf(text, "f - finish game");
		DrawString(screen, screen->w - 15 * 8, screen->h - 8 * 9, text, charset);
		sprintf(text, "\032 - left");
		DrawString(screen, screen->w - 15 * 8, screen->h - 7 * 9, text, charset);
		sprintf(text, "\033 - right");
		DrawString(screen, screen->w - 15 * 8, screen->h - 6 * 9, text, charset);
		sprintf(text, "\030 - faster");
		DrawString(screen, screen->w - 15 * 8, screen->h - 5 * 9, text, charset);
		sprintf(text, "\031 - slower");
		DrawString(screen, screen->w - 15 * 8, screen->h - 4 * 9, text, charset);
		sprintf(text, "space - shoot");
		DrawString(screen, screen->w - 15 * 8, screen->h - 3 * 9, text, charset);


		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		// handling of events (if there were any)
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				
				// accelerate / slower
				else if (event.key.keysym.sym == SDLK_UP) player->Accelerate();
				else if (event.key.keysym.sym == SDLK_DOWN) player->SlowDown();

				// left / right
				else if (event.key.keysym.sym == SDLK_LEFT) player->direction = -1;
				else if (event.key.keysym.sym == SDLK_RIGHT) player->direction = 1;

				// sort score by time
				else if (event.key.keysym.sym == SDLK_t) {
					if (!playing) {
						printf("sort time\n");
						SortRank(true);
						break;
					}
				}
				// pause the game or sort score by points
				else if (event.key.keysym.sym == SDLK_p) {
					if (!playing) {
						SortRank(false);
						break;
					}
					pause = true;
					pauseDown = true;
					while (pause) {

						while (SDL_PollEvent(&event)) {
							switch (event.type) {
							case SDL_KEYDOWN:
								if (event.key.keysym.sym == SDLK_p) {
									if (!pauseDown) {
										// continue time going,
										// otherwise 'delta' will influence items' movement after continuing game
										t2 = SDL_GetTicks();
										t1 = t2;
										pause = false;
									}
								}
								break;
							case SDL_KEYUP:
								if (event.key.keysym.sym == SDLK_p) {
									if (pauseDown)
										pauseDown = false;
								}
								break;
							}
						}
					}
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_n) {
					NewGame();
				}
				else if (event.key.keysym.sym == SDLK_LEFT) player->direction = 0;
				else if (event.key.keysym.sym == SDLK_RIGHT) player->direction = 0;
				if (event.key.keysym.sym == SDLK_p) {
					if (pauseDown)
						pauseDown = false;
				}
				//finish the game
				else if (event.key.keysym.sym == SDLK_f) {
					playing = false;
					WriteScore(score, worldTime);
					worldTime = 0;
					score = 0;
					player->Reset();
					for (auto& i : cars)
						i->Reset();
					for (auto& i : chasers)
						i->Reset();
					for (auto& i : attackers)
						i->Reset();
					player->lifes = 0;
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			};
		};
		frames++;
	}
}

void StartGame::NewGame() {
	playing = true;
	van->NewLife();
	player->ResetLife();
	score = 0;
	carScore = 0;
	worldTime = 0;
	unlimitLifeTime = UNLIMIT_LIFE_TIME;
	prevChaserBornTime = 0;
}

void StartGame::NPC_NotCrash() {
	// attacker
	for (auto& a : attackers) {
		if (a->active) {
			for (auto& c : chasers) {
				if (a->x < c->x + c->sprite->w / 2 &&
					a->x > c->x - c->sprite->w / 2 &&
					a->y > c->y - c->sprite->h - a->sprite->h / 2 &&
					a->y < c->y + c->sprite->h - a->sprite->h / 2)
				{
					c->x -= a->sprite->w / 2 * (c->x < a->x? 1 : -1);
				}
			}
			for (auto& c : cars) {
				if (a->x < c->x + c->sprite->w / 2 &&
					a->x > c->x - c->sprite->w / 2 &&
					a->y > c->y - c->sprite->h - a->sprite->h / 2 &&
					a->y < c->y + c->sprite->h - a->sprite->h / 2)
				{
					c->x -= a->sprite->w / 2 * (c->x < a->x ? 1 : -1);
				}
			}
		}

		// chaser
		for (auto& a : chasers) {
			if (a->active) {
				for (auto& c : cars) {
					if (a->x < c->x + c->sprite->w / 2 &&
						a->x > c->x - c->sprite->w / 2 &&
						a->y > c->y - c->sprite->h - a->sprite->h / 2 &&
						a->y < c->y + c->sprite->h - a->sprite->h / 2)
					{
						c->x -= a->sprite->w / 2 * (c->x < a->x ? 1 : -1);
					}
				}
			}
		}
	}
}

void StartGame::SortRank(bool isSortByTime) {
	if (rankNum == 0) return;

	// I rewrite the operator "<" and ">" in RankItem struct
	if (isSortByTime) {
		for (int i = 0; i < rankNum - 1; i++) {
			int maximum = i;
			for (int j = i; j < rankNum; j++) {
				if (rank[j] > rank[maximum]) {
					RankItem temp = rank[maximum];
					rank[maximum] = rank[j];
					rank[j] = temp;
				}
			}
		}
	}
	else {
		for (int i = 0; i < rankNum - 1; i++) {
			int maximum = i;
			for (int j = i; j < rankNum; j++) {
				if (rank[maximum] < rank[j]) {
					RankItem temp = rank[maximum];
					rank[maximum] = rank[j];
					rank[j] = temp;
				}
			}
		}
	}
}

// It is dynamic allocate size
// I make it limit "memory size" as 5 to make it looks better, 
// but it doesn't influence I use dynamic allocate memory
void StartGame::WriteScore(int newScore, int newTime) {
	RankItem ri(newScore, newTime);
	FILE* fp = NULL;
	fopen_s(&fp, RANK_FILE, "w");
	for (int i = 0; i < rankNum; i++) {
		if (rank[i] < ri) {
			if (rankNum < MEMORY_SIZE && rankNum > 0)
				rank = (RankItem*)realloc(rank, ++rankNum * sizeof(RankItem));

			for (int j = rankNum - 1; j > i; j--) {
				rank[j] = rank[j - 1];
			}
			rank[i] = ri;

			for (int k = 0; k < rankNum; k++) {
				fprintf(fp, "%d %d \n", rank[k].score, rank[k].time);
			}
			fclose(fp);
			return;
		}
	}

	if (rankNum == 0) {
		rankNum = 1;
		fprintf(fp, "%d %d \n", newScore, newTime);
		fclose(fp);
		return;
	}
	else {
		rank = (RankItem*)realloc(rank, ++rankNum * sizeof(RankItem));
		rank[rankNum - 1] = ri;
		for (int k = 0; k < rankNum; k++) {
			fprintf(fp, "%d %d \n", rank[k].score, rank[k].time);
		}
		fclose(fp);
		return;
	}
}

void StartGame::ReadScore() {
	rank = (RankItem*)malloc(sizeof(RankItem));
	FILE* fp;
	fp = fopen(RANK_FILE, "a+");

	while (!feof(fp))
	{
		text[0] = '\0';
		fgets(text, 100, fp); 
		if (text[0] != '\0') {
			rankNum++;
			rank = (RankItem*)realloc(rank, rankNum * sizeof(RankItem));
			rank[rankNum - 1] = GetIntFromStr();
			//printf("read: %d, %d\n", rank[rankNum - 1].score, rank[rankNum - 1].time);
		}
	}

	fclose(fp);
}

RankItem StartGame::GetIntFromStr() {
	int i = 0;
	int nScore = 0;
	int nTime = 0;
	while (text[i] != ' ') {
		nScore = nScore * 10 + (text[i++] - '0');
	}
	i++;
	while (text[i] != ' ') {
		nTime = nTime * 10 + (text[i++] - '0');
	}
	return RankItem(nScore, nTime);
}