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

TTF_Font* font_Title;
TTF_Font* font_Options;
SDL_Color textColor = { 255, 255, 255, 255 };
SDL_Color backgroundColor = { 0, 0, 0, 255 };

SDL_Texture* blendedTexture_Title;
SDL_Texture* blendedTexture_Play;
SDL_Texture* blendedTexture_HiSc;
SDL_Texture* blendedTexture_Exit;

SDL_Rect blendedRect_Title;
SDL_Rect blendedRect_Play;
SDL_Rect blendedRect_HiSc;
SDL_Rect blendedRect_Exit;

SDL_Rect windowRect = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 500 };
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

	SDL_RenderCopy(renderer, blendedTexture_Title, nullptr, &blendedRect_Title);
	SDL_RenderCopy(renderer, blendedTexture_Play, nullptr, &blendedRect_Play);
	SDL_RenderCopy(renderer, blendedTexture_HiSc, nullptr, &blendedRect_HiSc);
	SDL_RenderCopy(renderer, blendedTexture_Exit, nullptr, &blendedRect_Exit);

	SDL_RenderPresent(renderer);
}

bool SetupTTF(const std::string &fontName)
{
	if (TTF_Init() == -1)
	{
		cout << " Failed to initialize TTF : " << SDL_GetError() << std::endl;
		return false;
	}
	font_Title = TTF_OpenFont(fontName.c_str(), 90);
	if (font_Title == nullptr)
	{
		cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}
	font_Options = TTF_OpenFont(fontName.c_str(), 50);
	if (font_Options == nullptr)
	{
		cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void CreateTextTextures()
{
	//Title
	SDL_Surface* blended_Title = TTF_RenderText_Blended(font_Title, "*FLAPPY BIRD*", textColor);
	blendedTexture_Title = SurfaceToTexture(blended_Title);

	SDL_QueryTexture(blendedTexture_Title, NULL, NULL, &blendedRect_Title.w, &blendedRect_Title.h);
	blendedRect_Title.x = (windowRect.w - blendedRect_Title.w)/2;
	blendedRect_Title.y = 75;

	//Play
	SDL_Surface* blended_Play = TTF_RenderText_Blended(font_Options, "Play", textColor);
	blendedTexture_Play = SurfaceToTexture(blended_Play);

	SDL_QueryTexture(blendedTexture_Play, NULL, NULL, &blendedRect_Play.w, &blendedRect_Play.h);
	blendedRect_Play.x = (windowRect.w - blendedRect_Play.w) / 2;
	blendedRect_Play.y = blendedRect_Title.y + blendedRect_Title.h + 10;

	//Highscores
	SDL_Surface* blended_HiSc = TTF_RenderText_Blended(font_Options, "Highscores", textColor);
	blendedTexture_HiSc = SurfaceToTexture(blended_HiSc);

	SDL_QueryTexture(blendedTexture_HiSc, NULL, NULL, &blendedRect_HiSc.w, &blendedRect_HiSc.h);
	blendedRect_HiSc.x = (windowRect.w - blendedRect_HiSc.w) / 2;
	blendedRect_HiSc.y = blendedRect_Play.y + blendedRect_Play.h + 10;

	//Exit
	SDL_Surface* blended_Exit = TTF_RenderText_Blended(font_Options, "Exit", textColor);
	blendedTexture_Exit = SurfaceToTexture(blended_Exit);

	SDL_QueryTexture(blendedTexture_Exit, NULL, NULL, &blendedRect_Exit.w, &blendedRect_Exit.h);
	blendedRect_Exit.x = (windowRect.w - blendedRect_Exit.w) / 2;
	blendedRect_Exit.y = blendedRect_HiSc.y + blendedRect_HiSc.h + 10;
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
	TTF_CloseFont(font_Title);
	TTF_CloseFont(font_Options);
	SDL_Quit();
}