#pragma once
#include <SDL.h>
#include <vector>
#include "app.hpp"

class Bird
{
public:
  Bird(SDL_Renderer *renderer);
  ~Bird();
  void tick(bool isSpacePressed);
  void draw();
  float y = Application::Height / 2 - 50;
  const float x = Application::Width / 4 - 50;
private:
  SDL_Renderer *renderer;
  SDL_Texture *bird1;
  SDL_Texture *bird2;
  float v = 0.0f;
};

class Tube
{
public:
  Tube(SDL_Renderer *renderer, SDL_Texture *texture, int y, bool isUp);
  void tick();
  void draw();
private:
  SDL_Renderer *renderer;
  SDL_Texture *texture;
public:
  float x = Application::Width + 128;
  float y;
  bool isUp;
};

class Game
{
public:
  Game(SDL_Renderer *renderer);
  ~Game();
  int exec();
private:
  SDL_Renderer *renderer;
  SDL_Texture *tubeTexture;
  SDL_Texture *digitsTexture;
  Bird bird;
  std::vector<Tube> tubeList;
  bool tick(bool isSpacePressed);
  int counter = 0;
  void drawTubeCounter();
};