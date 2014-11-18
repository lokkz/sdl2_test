// main.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

void toggle_fs();
SDL_Window *win;
Uint32 flags;


int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}


	SDL_DisplayMode dm;

	for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
	{
		int res = SDL_GetDesktopDisplayMode(0, &dm);
		if(res != 0)
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
		else
			SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, dm.w, dm.h, dm.refresh_rate);
	}

	win = SDL_CreateWindow("Hello /gd/", 0, 0, 800, 600, SDL_WINDOW_SHOWN);
	flags = (SDL_GetWindowFlags(win) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (win == nullptr)
	{
		SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
		SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//SDL_Delay(2000);
	SDL_Event ev;
	bool quit = 0;

	SDL_Surface *srf = SDL_LoadBMP("bkg1.bmp");
	if(srf == nullptr)
		SDL_Log("SDL_LoadBMP Error: %s", SDL_GetError());
	SDL_Texture *bkg = SDL_CreateTextureFromSurface(ren, srf);
	SDL_FreeSurface(srf);
	SDL_RenderCopy(ren, bkg, NULL, NULL);
	SDL_RenderPresent(ren);
	

	while(!quit)
	{
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				quit = 1;
			else if(ev.type == SDL_KEYDOWN)
			{
				if(ev.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
				if(ev.key.keysym.sym == SDLK_F10)
					toggle_fs();
			}
		}
	}

	SDL_DestroyTexture(bkg);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

    return 0;
}

void toggle_fs()
{
	flags = (SDL_GetWindowFlags(win) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (SDL_SetWindowFullscreen(win, flags) < 0)
		SDL_Log("Toggling fullscreen mode failed: %s", SDL_GetError());
	SDL_Log("flags: %i", flags);
	if((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		SDL_SetWindowFullscreen(win, flags);
	else
		SDL_SetWindowFullscreen(win, 1);
}

//EOF
