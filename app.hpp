#pragma once
#include "Header1.hpp"

class Application
{
public:
  static const auto Width = 1000;
  static const auto Height = 500;
  Application();
  int exec();
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
};