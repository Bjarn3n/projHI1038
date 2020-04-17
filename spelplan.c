#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "spelplan.h"

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
    *window = SDL_CreateWindow("Minesweeper+", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if(*window == NULL){
        printf("Error creating window. Error: %s\n", SDL_GetError());
        return 0;
    } else {
        printf("Window created successfully!\n");        
        *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
        if(*renderer == NULL){
            printf("Error creating renderer. Error: %s\n", SDL_GetError());
            return 0;
        }
        else printf("Renderer created successfully!\n");
    }    	
    return 1;
}

void closeProgram(SDL_Window **window, SDL_Renderer **renderer){
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window); //Stäng ner fönstret
    *window = NULL;
    *renderer = NULL;
    SDL_Quit(); //"Stäng av" SDL
}
