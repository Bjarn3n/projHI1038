#ifndef _SPELPLAN_H
#define _SPELPLAN_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
void closeProgram(SDL_Window **window, SDL_Renderer **renderer);
#endif
