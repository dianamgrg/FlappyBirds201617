#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
using namespace std;

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

bool SetupTTF(const std::string &fontName);
SDL_Texture* SurfaceToTexture(SDL_Surface* surf);
void CreateTextTextures();

void Render();
void Menu();

TTF_Font* font;
SDL_Color textColor = { 255, 255, 255, 255 };
SDL_Color backgroundColor = { 0, 0, 0, 255 };

SDL_Texture* solidTexture;
SDL_Texture* blendedTexture;
SDL_Texture* shadedTexture;

SDL_Rect solidRect;
SDL_Rect blendedRect;
SDL_Rect shadedRect;

SDL_Rect windowRect = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720 };
SDL_Window* window;
SDL_Renderer* renderer;

void CloseEverything();

int main(int argc, char* args[])
{
	if (!InitEverything())
	{
		cout << "ERROR" << '\n';
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
			Menu();
		}

	}
	CloseEverything();
	return 0;
}
void Menu()
{
	Render();
}
void Render()
{
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, blendedTexture, nullptr, &blendedRect);

	SDL_RenderPresent(renderer);
}

bool SetupTTF(const std::string &fontName)
{
	if (TTF_Init() == -1)
	{
		cout << " Failed to initialize TTF : " << SDL_GetError() << std::endl;
		return false;
	}
	font = TTF_OpenFont(fontName.c_str(), 90);
	if (font == nullptr)
	{
		cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void CreateTextTextures()
{
	SDL_Surface* blended = TTF_RenderText_Blended(font, "*FLAPPY BIRD*", textColor);
	blendedTexture = SurfaceToTexture(blended);

	SDL_QueryTexture(blendedTexture, NULL, NULL, &blendedRect.w, &blendedRect.h);
	blendedRect.x = (windowRect.w - blendedRect.w)/2;
	blendedRect.y = solidRect.y + solidRect.h + 20;
}

SDL_Texture* SurfaceToTexture(SDL_Surface* surf)
{
	SDL_Texture* text;

	text = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_FreeSurface(surf);

	return text;
}

bool InitEverything()
{
	if (!InitSDL())
		return false;

	if (!CreateWindow())
		return false;

	if (!CreateRenderer())
		return false;

	SetupRenderer();

	if (!SetupTTF("courbd.ttf"))
		return false;

	CreateTextTextures();

	return true;
}

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

bool CreateWindow()
{
	window = SDL_CreateWindow("Flappy Bird", windowRect.x, windowRect.y, windowRect.w, windowRect.h, 0);

	if (window == nullptr)
	{
		cout << "Failed to create window : " << SDL_GetError();
		return false;
	}

	return true;
}

bool CreateRenderer()
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	return true;
}

void SetupRenderer()
{
	SDL_RenderSetLogicalSize(renderer, windowRect.w, windowRect.h);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
void CloseEverything()
{
	SDL_DestroyWindow(window);
	window = NULL;
	TTF_CloseFont(font);
	SDL_Quit();
}