#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
class RenderWindow
{
public:
	RenderWindow();
	void create(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void cleanUp();
	void clear();
	void render(const SDL_Rect* rect, const short r, const short g, const short b, const short a);
	void render(int p_x, int p_y, const char* p_text, TTF_Font* font, SDL_Color p_color);
	void display();
	void changeTitle(const char* p_title);
	void setColor(int r, int g, int b, int a);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};
