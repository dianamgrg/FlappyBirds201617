#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using std::string;

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();
void SetupRenderer2();
void SetupRenderer3();

bool SetupTTF(const std::string &fontName);
SDL_Texture* SurfaceToTexture(SDL_Surface* surf);
void CreateTextTextures();

void RenderMenu();
void RenderHiSc();
void RenderGame();
void Menu();
void Game();
void Highscores();
int hisc;

TTF_Font* font_Title;
TTF_Font* font_Options;
SDL_Color textColor = { 255, 255, 255, 255 };
SDL_Color textColorMouseover = { 255, 0, 0, 255 };
SDL_Color backgroundColor = { 0, 0, 0, 255 };

SDL_Texture* blendedTexture_Title;
SDL_Texture* blendedTexture_Play;
SDL_Texture* blendedTexture_HiSc;
SDL_Texture* blendedTexture_Exit;

SDL_Texture* hiscBT;
SDL_Texture* hiscBT1;
SDL_Texture* hiscBT2;
SDL_Texture* hiscBT3;

SDL_Rect blendedRect_Title;
SDL_Rect blendedRect_Play;
SDL_Rect blendedRect_HiSc;
SDL_Rect blendedRect_Exit;

SDL_Rect hiscBR;
SDL_Rect hiscBR1;
SDL_Rect hiscBR2;
SDL_Rect hiscBR3;

SDL_Rect windowRect = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 500 };
SDL_Window* window;
SDL_Rect windowRect2 = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 500 };
SDL_Window* window2;
SDL_Rect windowRect3 = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 500 };
SDL_Window* window3;
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
	RenderMenu();
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_1:
				Game();
				break;

			case SDLK_2:
				Highscores();
				break;

			case SDLK_3:
				CloseEverything();
				break;
			}
		}
	}
}
void Game()
{
	RenderGame();
}
void Highscores()
{
	SDL_DestroyWindow(window);
	window = NULL;
	window3 = SDL_CreateWindow("Highscores", windowRect3.x, windowRect3.y, windowRect3.w, windowRect3.h, 0);
	SetupRenderer3();
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		RenderHiSc();
	}
	CloseEverything();
}
void RenderMenu()
{
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, blendedTexture_Title, nullptr, &blendedRect_Title);
	SDL_RenderCopy(renderer, blendedTexture_Play, nullptr, &blendedRect_Play);
	SDL_RenderCopy(renderer, blendedTexture_HiSc, nullptr, &blendedRect_HiSc);
	SDL_RenderCopy(renderer, blendedTexture_Exit, nullptr, &blendedRect_Exit);

	SDL_RenderPresent(renderer);
}
void RenderHiSc()
{
	ifstream f("HiSc.txt");
	if (f.is_open())
	{
		f.seekg(0, ios::end);
		size_t size = f.tellg();
		if (size == 0)
		{
			SDL_Surface* hiscB = TTF_RenderText_Blended(font_Title, "Highscores", textColor);
			hiscBT = SurfaceToTexture(hiscB);

			SDL_QueryTexture(hiscBT, NULL, NULL, &hiscBR.w, &hiscBR.h);
			hiscBR.x = (windowRect3.w - hiscBR.w) / 2;
			hiscBR.y = 75;

			SDL_Surface* hiscB1 = TTF_RenderText_Blended(font_Options, "Be the first to play the game!", textColor);
			hiscBT1 = SurfaceToTexture(hiscB1);

			SDL_QueryTexture(hiscBT1, NULL, NULL, &hiscBR1.w, &hiscBR1.h);
			hiscBR1.x = (windowRect3.w - hiscBR1.w) / 2;
			hiscBR1.y = hiscBR.y + hiscBR.h + 10;

			SDL_RenderClear(renderer);

			SDL_RenderCopy(renderer, hiscBT, nullptr, &hiscBR);
			SDL_RenderCopy(renderer, hiscBT1, nullptr, &hiscBR1);

			SDL_RenderPresent(renderer);
		}
		else
		{
			int h1, h2, h3;
			f >> h1 >> h2 >> h3;
			std::string hs1, hs2, hs3;
			hs1 = "1. " + std::to_string(h1);
			hs2 = "2. " + std::to_string(h2);
			hs3 = "3. " + std::to_string(h3);
			f.close();
			const char * c1 = hs1.c_str();
			const char * c2 = hs2.c_str();
			const char * c3 = hs3.c_str();

			SDL_Surface* hiscB = TTF_RenderText_Blended(font_Title, "Highscores", textColor);
			hiscBT = SurfaceToTexture(hiscB);

			SDL_QueryTexture(hiscBT, NULL, NULL, &hiscBR.w, &hiscBR.h);
			hiscBR.x = (windowRect3.w - hiscBR.w) / 2;
			hiscBR.y = 75;

			SDL_Surface* hiscB1 = TTF_RenderText_Blended(font_Options, c1, textColor);
			hiscBT1 = SurfaceToTexture(hiscB1);

			SDL_QueryTexture(hiscBT1, NULL, NULL, &hiscBR1.w, &hiscBR1.h);
			hiscBR1.x = (windowRect3.w - hiscBR1.w) / 2;
			hiscBR1.y = hiscBR.y + hiscBR.h + 10;

			SDL_Surface* hiscB2 = TTF_RenderText_Blended(font_Options, c2, textColor);
			hiscBT2 = SurfaceToTexture(hiscB2);

			SDL_QueryTexture(hiscBT2, NULL, NULL, &hiscBR2.w, &hiscBR2.h);
			hiscBR2.x = (windowRect3.w - hiscBR2.w) / 2;
			hiscBR2.y = hiscBR1.y + hiscBR1.h + 10;

			SDL_Surface* hiscB3 = TTF_RenderText_Blended(font_Options, c3, textColor);
			hiscBT3 = SurfaceToTexture(hiscB3);

			SDL_QueryTexture(hiscBT3, NULL, NULL, &hiscBR3.w, &hiscBR3.h);
			hiscBR3.x = (windowRect3.w - hiscBR3.w) / 2;
			hiscBR3.y = hiscBR2.y + hiscBR2.h + 10;

			SDL_RenderClear(renderer);

			SDL_RenderCopy(renderer, hiscBT, nullptr, &hiscBR);
			SDL_RenderCopy(renderer, hiscBT1, nullptr, &hiscBR1);
			SDL_RenderCopy(renderer, hiscBT2, nullptr, &hiscBR2);
			SDL_RenderCopy(renderer, hiscBT3, nullptr, &hiscBR3);

			SDL_RenderPresent(renderer);
		}
		f.close();
	}
}
void RenderGame()
{

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
	SDL_Surface* blended_Play = TTF_RenderText_Blended(font_Options, "1 - Play", textColor);
	blendedTexture_Play = SurfaceToTexture(blended_Play);

	SDL_QueryTexture(blendedTexture_Play, NULL, NULL, &blendedRect_Play.w, &blendedRect_Play.h);
	blendedRect_Play.x = (windowRect.w - blendedRect_Play.w) / 2;
	blendedRect_Play.y = blendedRect_Title.y + blendedRect_Title.h + 10;

	//Highscores
	SDL_Surface* blended_HiSc = TTF_RenderText_Blended(font_Options, "2 - Highscores", textColor);
	blendedTexture_HiSc = SurfaceToTexture(blended_HiSc);

	SDL_QueryTexture(blendedTexture_HiSc, NULL, NULL, &blendedRect_HiSc.w, &blendedRect_HiSc.h);
	blendedRect_HiSc.x = (windowRect.w - blendedRect_HiSc.w) / 2;
	blendedRect_HiSc.y = blendedRect_Play.y + blendedRect_Play.h + 10;

	//Exit
	SDL_Surface* blended_Exit = TTF_RenderText_Blended(font_Options, "3 - Exit", textColor);
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
void SetupRenderer2()
{
	renderer = SDL_CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, windowRect2.w, windowRect2.h);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
void SetupRenderer3()
{
	renderer = SDL_CreateRenderer(window3, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, windowRect3.w, windowRect3.h);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
void CloseEverything()
{
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyWindow(window2);
	window2 = NULL;
	SDL_DestroyWindow(window3);
	window3 = NULL;
	TTF_CloseFont(font_Title);
	TTF_CloseFont(font_Options);
	SDL_Quit();
}