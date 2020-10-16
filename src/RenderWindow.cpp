#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "RenderWindow.h"

RenderWindow::RenderWindow()
{}

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
		return;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture (" << p_filePath << ") Error: " << SDL_GetError() << std::endl;

	return texture;
}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}

void RenderWindow::render(const SDL_Rect* rect, const short r, const short g, const short b, const short a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, rect);
}

void RenderWindow::render(int p_x, int p_y, const char* p_text, TTF_Font* font, SDL_Color p_color)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, p_text, p_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = surface->w;
	src.h = surface->h;

	SDL_Rect dst;
	dst.x = p_x-src.w/2;
	dst.y = p_y-src.h/2;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, texture, &src, &dst);
	SDL_FreeSurface(surface);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::setColor(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer,r, g, b, a);
}

void RenderWindow::changeTitle(const char* p_title)
{
	SDL_SetWindowTitle(window, p_title);
}
