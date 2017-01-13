#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Header1.hpp"
#include "app.hpp"
#include <stdexcept>

using namespace std;
using std::string;
int hisc;

bool InitEverything();
bool InitSDL();
bool CreateWindow1();
bool CreateWindow3();

bool SetupTTF(const std::string &fontName);
SDL_Texture* SurfaceToTexture(SDL_Surface* surf);
SDL_Texture* LoadTextureFromFile(SDL_Renderer *renderer, char const *fileName);
void CreateTextTextures();

void RenderMenu();
void RenderHiSc();
void RenderGame();
void Menu();
void Game();
void Highscores();
void CloseEverything();

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
SDL_Window* window2;
SDL_Rect windowRect3 = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 500 };
SDL_Window* window3;
SDL_Renderer* renderer;

bool isSpacePressed()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0)
		if (ev.type == SDL_KEYDOWN)
			if (ev.key.keysym.sym == SDLK_SPACE)
				return true;
	return false;
}

int Run();

Application::Application()
{
	auto res = SDL_Init(SDL_INIT_EVERYTHING);
	if (res != 0)
		throw std::runtime_error(std::string("SDL_Init(SDL_INIT_EVERYTHING)") + SDL_GetError());
	SDL_CreateWindowAndRenderer(Width, Height, SDL_WINDOW_BORDERLESS, &window, &renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetWindowPosition(window, 65, 126);
}

int Application::exec()
{
	for (;;)
	{
		Application Game;
		if (Game.exec() == 0)
			break;
	}
	return 0;
}

Bird::Bird(SDL_Renderer *renderer):
	renderer(renderer),
	bird1(LoadTextureFromFile(renderer, "bird1.bmp")),
	bird2(LoadTextureFromFile(renderer, "bird2.bmp"))
{}

Bird::~Bird()
{
	SDL_DestroyTexture(bird1);
	SDL_DestroyTexture(bird2);
}


void Bird::draw()
{
	SDL_Rect r;
	r.x = x - 64;
	r.y = y - 64;
	r.w = 128;
	r.h = 128;
	auto res = SDL_RenderCopyEx(renderer, SDL_GetTicks() % 300 > 150 ? bird1 : bird2, nullptr, &r, v * 50, nullptr, SDL_FLIP_NONE);
	if (res != 0)
		throw std::runtime_error(std::string("SDL_RenderCopy ") + SDL_GetError());
}

void Bird::tick(bool isSpacePressed)
{
	v -= isSpacePressed ? 0.003 : 0;
	v += 0.001;
	y += v;
}

Tube::Tube(SDL_Renderer *renderer, SDL_Texture *texture, int y, bool isUp) :
	renderer(renderer),
	texture(texture),
	y(y),
	isUp(isUp)
{}

void Tube::tick()
{
	x -= 0.25f;
}

void Tube::draw()
{
	if (!isUp)
	{
		SDL_Rect r;
		r.x = x - 128;
		r.y = y;
		r.w = 256;
		r.h = 720;
		auto res = SDL_RenderCopyEx(renderer, texture, nullptr, &r, 0, nullptr, SDL_FLIP_NONE);
		if (res != 0)
			throw std::runtime_error(std::string("SDL_RenderCopy ") + SDL_GetError());
	}
	else
	{
		SDL_Rect r;
		r.x = x - 128;
		r.y = y - Application::Height;
		r.w = 256;
		r.h = 720;
		auto res = SDL_RenderCopyEx(renderer, texture, nullptr, &r, 0, nullptr, SDL_FLIP_VERTICAL);
		if (res != 0)
			throw std::runtime_error(std::string("SDL_RenderCopy ") + SDL_GetError());
	}

}

Game::Game(SDL_Renderer *renderer) :
	renderer(renderer),
	tubeTexture(LoadTextureFromFile(renderer, "tube.bmp")),
	digitsTexture(LoadTextureFromFile(renderer, "digits.bmp")),
	bird(renderer)
{
}

Game::~Game()
{
	SDL_DestroyTexture(tubeTexture);
	SDL_DestroyTexture(digitsTexture);
}


bool Game::tick(bool isSpacePressed)
{
	if (counter++ % 2000 == 0)
	{
		auto y = rand() % (Application::Height - 200 - 250 - 100) + 100;
		tubeList.emplace_back(renderer, tubeTexture, y + 250, false);
		tubeList.emplace_back(renderer, tubeTexture, y, true);
	}
	bird.tick(isSpacePressed);
	SDL_Rect birdRect;
	birdRect.x = bird.x - 114 / 2;
	birdRect.y = bird.y - 94 / 2;
	birdRect.w = 114;
	birdRect.h = 94;
	SDL_Rect res;
	for (auto &tube : tubeList)
	{
		tube.tick();
		SDL_Rect tubeRect;
		tubeRect.x = tube.x - 128 + 20;
		tubeRect.w = 256 - 40;
		tubeRect.h = 10000;
		if (tube.isUp)
		{
			tubeRect.y = tube.y - 10000;
			if (SDL_IntersectRect(&birdRect, &tubeRect, &res))
			{
				return false;
			}
		}
		else
		{
			tubeRect.y = tube.y;
			if (SDL_IntersectRect(&birdRect, &tubeRect, &res))
			{
				return false;
			}
		}
	}
	for (auto iter = std::begin(tubeList); iter != std::end(tubeList); )
	{
		if (iter->x < -128)
			iter = tubeList.erase(iter);
		else
			++iter;
	}
	return bird.y < Application::Height;
}


int Game::exec()
{
	auto oldTick = SDL_GetTicks();
	bool isSpacePressed = false;
	for (auto done = false; !done;)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				isSpacePressed = true;
				break;
			case SDL_MOUSEBUTTONUP:
				isSpacePressed = false;
				break;
			case SDL_QUIT:
				done = true;
				break;
			}
		}

		auto currentTick = SDL_GetTicks();
		for (auto i = oldTick; i < currentTick; ++i)
		{
			if (!tick(isSpacePressed))
			{
				SDL_Delay(4000);
				return 1;
			}
		}
		oldTick = currentTick;

		SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);
		bird.draw();
		for (auto &tube : tubeList)
			tube.draw();
		drawTubeCounter();
		SDL_RenderPresent(renderer);
	}
	return 0;
}

void Game::drawTubeCounter()
{
	int num = (counter - 4 * (Application::Width - bird.x)) / 2000 + 1;
	if (num < 0)
		num = 0;
	SDL_Rect srcRect;
	srcRect.y = 0;
	srcRect.w = 34;
	srcRect.h = 64;
	SDL_Rect destRect;
	destRect.x = Application::Width - 32 - 50;
	destRect.y = 50;
	destRect.w = 34;
	destRect.h = 64;
	do
	{
		srcRect.x = num % 10 * 34;
		num /= 10;
		SDL_RenderCopy(renderer, digitsTexture, &srcRect, &destRect);
		destRect.x -= 34;
	} while (num > 0);
}

int Run()
{
	Application app;
	return app.exec();
}

void Menu()
{
	RenderMenu();
	SDL_Event e2;
	while (SDL_PollEvent(&e2) != 0)
	{
		if (e2.type == SDL_KEYDOWN)
		{
			switch (e2.key.keysym.sym)
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
void Highscores()
{
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_RenderClear(renderer);
	CreateWindow3();
	SDL_Event e4;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e4) != 0)
		{
			if (e4.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		RenderHiSc();
	}
	CloseEverything();
}
void Game()
{
	SDL_DestroyWindow(window);
	window = NULL;
	if (hisc == 1)
	{
		SDL_DestroyWindow(window3);
		window3 = NULL;
	}
	/*SDL_RenderClear(renderer);
	CreateWindow2();
	SDL_Event e3;
	bool quit = false;
	int frame = 0;
	Timer fps;
	while (!quit)
	{
	fps.start();
	while (SDL_PollEvent(&e3) != 0)
	{
	if (e3.type == SDL_QUIT)
	{
	quit = true;
	}
	}
	RenderGame();
	frame++;
	if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
	{
	SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
	}
	}*/
	Run();
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

			SDL_Event e5;
			while (SDL_PollEvent(&e5) != 0)
			{
				if (e5.type == SDL_KEYDOWN)
				{
					if (e5.key.keysym.sym == SDLK_RETURN)
					{
						hisc = 1;
						Game();
						break;
					}
				}
			}
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
	blendedRect_Title.x = (windowRect.w - blendedRect_Title.w) / 2;
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

	if (!CreateWindow1())
		return false;

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

bool CreateWindow1()
{
	window = SDL_CreateWindow("Flappy Bird", windowRect.x, windowRect.y, windowRect.w, windowRect.h, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, windowRect.w, windowRect.h);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	if (window == nullptr || renderer == nullptr)
	{
		cout << "Failed: " << SDL_GetError();
		return false;
	}

	return true;
}
bool CreateWindow3()
{
	window3 = SDL_CreateWindow("Highscores", windowRect3.x, windowRect3.y, windowRect3.w, windowRect3.h, 0);
	renderer = SDL_CreateRenderer(window3, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, windowRect3.w, windowRect3.h);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	if (window3 == nullptr || renderer == nullptr)
	{
		cout << "Failed: " << SDL_GetError();
		return false;
	}

	return true;
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