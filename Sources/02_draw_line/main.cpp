#include <iostream>

#include <SDL.h>

static void Render (SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
	SDL_RenderClear (renderer);

	SDL_SetRenderDrawColor (renderer, 0, 0, 200, 255);
	SDL_RenderDrawLine (renderer, 10, 10, 200, 100);

	SDL_RenderPresent (renderer);
}

static bool MainLoop (SDL_Renderer* renderer)
{
	SDL_Event sdlEvent;
	if (SDL_PollEvent (&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				return false;
		}
	}

	Render (renderer);
	return true;
}

int main (int, char**)
{
	SDL_Init (SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow ("Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer (window, -1, 0);

	while (true) {
		if (!MainLoop (renderer)) {
			break;
		}
	}

	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();

	return EXIT_SUCCESS;
}
