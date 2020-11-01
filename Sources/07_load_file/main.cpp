#include <iostream>
#include <vector>
#include <fstream>

#include <SDL.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

struct Line
{
	SDL_Point beg;
	SDL_Point end;
};

static std::vector<Line> lines;
static bool inInteraction = false;

static SDL_Renderer* gRenderer = nullptr;

static void Render (SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
	SDL_RenderClear (renderer);

	SDL_SetRenderDrawColor (renderer, 0, 0, 200, 255);
	for (const Line& line : lines) {
		SDL_RenderDrawLine (renderer, line.beg.x, line.beg.y, line.end.x, line.end.y);
	}

	SDL_RenderPresent (renderer);
}

static bool MainLoop (SDL_Renderer* renderer)
{
	SDL_Event sdlEvent;
	if (SDL_PollEvent (&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_WINDOWEVENT:
				Render (renderer);
				break;
			case SDL_QUIT:
				return false;
			case SDL_MOUSEBUTTONDOWN:
				{
					if (sdlEvent.button.button == 1) {
						Line line {
							{ sdlEvent.button.x, sdlEvent.button.y },
							{ sdlEvent.button.x, sdlEvent.button.y }
						};
						lines.push_back (line);
						inInteraction = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				{
					if (sdlEvent.button.button == 1) {
						inInteraction = false;
#ifdef EMSCRIPTEN
						EM_ASM ({
							LineCreated ($0);
						}, lines.size ());
#endif
					}
				}
				break;
			case SDL_MOUSEMOTION:
				{
					if (inInteraction) {
						Line& lastLine = lines.back ();
						lastLine.end.x = sdlEvent.motion.x;
						lastLine.end.y = sdlEvent.motion.y;
					}
					Render (renderer);
				}
				break;
		}
	}

	return true;
}

#ifdef EMSCRIPTEN
static void EmscriptenMainLoop (void* arg)
{
	SDL_Renderer* renderer = (SDL_Renderer*) arg;
	MainLoop (renderer);
}
#endif

extern "C"
{

void ClearLines ()
{
	if (gRenderer != nullptr) {
		lines.clear ();
		Render (gRenderer);
	}
}

};

static void LoadLinesFromFile (std::vector<Line>& resultLines)
{
	std::ifstream file ("Assets/lines.txt");
	if (file.is_open ()) {
		int lineCount = 0;
		file >> lineCount;
		for (int i = 0; i < lineCount; i++) {
			Line line;
			file >> line.beg.x; file >> line.beg.y;
			file >> line.end.x; file >> line.end.y;
			resultLines.push_back (line);
		}
		file.close ();
	}
}

int main (int, char**)
{
	SDL_Init (SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow ("Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer (window, -1, 0);
	
	LoadLinesFromFile (lines);

	gRenderer = renderer;
#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg (EmscriptenMainLoop, renderer, 0, true);
#else
	while (true) {
		if (!MainLoop (renderer)) {
			break;
		}
	}
#endif
	gRenderer = nullptr;

	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();

	return EXIT_SUCCESS;
}
