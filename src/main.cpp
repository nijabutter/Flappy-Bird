#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "RenderWindow.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CENTER_X = SCREEN_WIDTH / 2;
const int CENTER_Y = SCREEN_HEIGHT / 2;
const int GRAVITY = 10;
const int SPEED = 15;
const int SCROLL_SPEED = 2;
const int PIPE_GAP = 150;
const int PIPE_WIDTH = 100;

const int SAND_HEIGHT = 80;
const int GRASS_HEIGHT = SAND_HEIGHT / 6;
int velocityY = GRAVITY;
int score = 0;
RenderWindow window;

bool showDeath = false;

bool gameRunning = true;

bool w_down = false;
bool a_down = false;
bool s_down = false;
bool d_down = false;

SDL_Rect player;
SDL_Rect sand;
SDL_Rect grass;

TTF_Font* font;

std::vector<SDL_Rect> pipes;
int pipeCount = 0;

short death = 0;

void newPipe(int x = SCREEN_WIDTH)
{
	int max = CENTER_Y;
	int min = CENTER_Y/2;

	int topheight = rand() % (max - min) + min;
	int gap = rand() % (PIPE_GAP*2 - PIPE_GAP) + PIPE_GAP;
	int bottom_y = topheight + gap;
	int bottom_h = SCREEN_HEIGHT - bottom_y;

	SDL_Rect top;
	top.x = x;
	top.y = 0;
	top.w = PIPE_WIDTH;
	top.h = topheight;

	SDL_Rect bottom;
	bottom.x = x;
	bottom.y = bottom_y;
	bottom.w = PIPE_WIDTH;
	bottom.h = bottom_h;

	pipes.push_back(top);
	pipes.push_back(bottom);
	pipeCount+=2;

	// bottom y =  get height plus pipe gap
	// bottom height = bottom y - screen height

}


void init()
{
	srand (time(NULL));

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	window.create("Flappy Bird", SCREEN_WIDTH, SCREEN_HEIGHT);
	font = TTF_OpenFont("res/fonts/Cozette.ttf", 100);

	player.x = CENTER_X/2;
	player.y = CENTER_Y;
	player.w = 20;
	player.h = 20;
	newPipe(CENTER_X+CENTER_X/2);
	newPipe();
	newPipe(SCREEN_WIDTH + CENTER_X/2);
	newPipe(SCREEN_WIDTH + CENTER_X);
	sand.x = 0;
	sand.y = SCREEN_HEIGHT - SAND_HEIGHT;
	sand.w = SCREEN_WIDTH;
	sand.h = SAND_HEIGHT;

	grass.x = 0;
	grass.y = sand.y;
	grass.w = SCREEN_WIDTH;
	grass.h = GRASS_HEIGHT;
}

void Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				gameRunning = false;
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
						velocityY = -SPEED;
						break;
					}
					default: break;
				}
			}
			default:
			{
				break;
			}
		}
	}
}


void reset()
{
	score = 0;
	pipeCount = 0;
	pipes.clear();
	newPipe(CENTER_X+CENTER_X/2);
	newPipe();
	newPipe(SCREEN_WIDTH + CENTER_X/2);
	newPipe(SCREEN_WIDTH + CENTER_X);
	player.x = CENTER_X/2;
	player.y = CENTER_Y;
	velocityY = GRAVITY;

}

void DeathScreen()
{
	while (death > 0)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					gameRunning = false;
					death = 0;
					break;
				}
				case SDL_KEYDOWN:
				{
				switch (event.key.keysym.scancode)
				{
					case SDL_SCANCODE_R:
					{
						death = 0;
						reset();
						break;
					}
					default: {break;}
				}
				break;
				}
				default:
				{
					break;
				}
			}
		}
		window.clear();
		window.render(&player, 210, 190, 40, 255);

		for (SDL_Rect& p : pipes)
		{
			window.render(&p, 100, 150, 40, 255);
		}
		window.render(&sand, 220, 215, 150, 255);
		window.render(&grass, 160, 230, 90, 255);

		window.render(CENTER_X, SCREEN_HEIGHT - SAND_HEIGHT / 2 + 10, std::to_string(score).c_str(), font, {200, 190, 90, 255});
		switch (death)
		{
			case 1:
			window.render(CENTER_X, CENTER_Y, "Thats a pipe", font, {255, 255, 255, 255});
			break;
			case 2:
			window.render(CENTER_X, CENTER_Y, "You have wings you know?", font, {255, 255, 255, 255});
			break;
			default:
			break;
		}
		window.render(CENTER_X, CENTER_Y+CENTER_Y/2, "Press R to restart", font, {255, 255, 255, 255});
		window.setColor(110, 200, 200, 255);
		window.display();
	}

}

void Update()
{
	if (velocityY < 0)
		velocityY++;
	else if (velocityY < GRAVITY)
		velocityY++;
	player.y += velocityY;

	if (player.y < 0)
		player.y = 0;
	else if (player.y + player.h > sand.y) {
		player.y = sand.y - player.h;
		death = 2;
	}

	for (int i = 0; i < pipeCount; i++)
	{
		pipes[i].x -= SCROLL_SPEED;
		int bottomRightCornerX = player.x + player.w;
		int bottomRightCornerY = player.y + player.h;

		if (player.x >= pipes[i].x && player.x <= pipes[i].x+pipes[i].w && player.y >= pipes[i].y && player.y <= pipes[i].y+pipes[i].h)
		{
			death = 1;
			std::cout<<"Game over"<<std::endl;
		}
		else if (bottomRightCornerX >= pipes[i].x && bottomRightCornerX <= pipes[i].x+pipes[i].w && bottomRightCornerY >= pipes[i].y && bottomRightCornerY <= pipes[i].y+pipes[i].h)
		{
			death = 1;
		}
		if (player.x + player.w/2 == pipes[i].x + pipes[i].w/2 && i % 2 == 0)
		{
			score++;
		}

		else if (pipes[i].x + pipes[i].w <= 0) {
			pipes.erase(pipes.begin() + 0);
			pipes.erase(pipes.begin() + 0);
			pipeCount-=2;
			newPipe();
		}
	}
}

void Render()
{
	window.clear();
	window.render(&player, 210, 190, 40, 255);

	for (SDL_Rect& p : pipes)
	{
		window.render(&p, 100, 150, 40, 255);
	}
	window.render(&sand, 220, 215, 150, 255);
	window.render(&grass, 160, 230, 90, 255);
	window.render(CENTER_X, SCREEN_HEIGHT - SAND_HEIGHT / 2 + 10, std::to_string(score).c_str(), font, {200, 190, 90, 255});
	window.setColor(110, 200, 200, 255);
	window.display();
}

int main(int argc, char* args[])
{
	init();
	while (gameRunning) {
		if (death > 0) {
			DeathScreen();
		}
		Input();
		Update();
		Render();
		SDL_Delay(16);
	}
	window.cleanUp();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
