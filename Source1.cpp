#include <SDL.h>
#include <iostream>
using namespace std;

SDL_Window* Window1 = NULL;
SDL_Surface* ScreenSurface1 = NULL;

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << SDL_GetError();
		success = false;
	}
	else
	{
		Window1 = SDL_CreateWindow("FLAPPY BIRD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
		if (Window1 == NULL)
		{
			cout << SDL_GetError();
			success = false;
		}
		else
		{
			ScreenSurface1 = SDL_GetWindowSurface(Window1);
		}
	}

	return success;
}
void close()
{
	SDL_FreeSurface(ScreenSurface1);
	SDL_DestroyWindow(Window1);
	Window1 = NULL;
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init())
	{
		cout<<"ERROR"<<'\n';
	}
	else
	{
		bool quit = false;
		SDL_Event e;
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}
			SDL_FillRect(ScreenSurface1, NULL, SDL_MapRGB(ScreenSurface1->format, 0x00, 0x00, 0x00));
			SDL_UpdateWindowSurface(Window1);
		}
		
	}
	close();
	return 0;
}