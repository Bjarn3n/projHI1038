#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "tiles.h"
#include "spelplan.h"
#include "networkS.h"


int main(int argc, char *args[]) {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER|SDL_INIT_EVERYTHING) != 0) {
      printf("error initializing SDL: %s\n", SDL_GetError());
      return 0;
  	}
	char *p;
	int row,col;
	if(argc==3){
		long r = strtol(args[1],&p,10);
		row = r;
		long c = strtol(args[2],&p,10);
		col = c; 
	}
	else{
		row = NR_OF_ROWS;
		col = NR_OF_COLS;
	}  
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if(!init(&window,&renderer,(col*TILE_PIXEL_SIZE),row*(TILE_PIXEL_SIZE))){
		printf("Error occured, exiting!");
		return 0;
	}
	if(TTF_Init() < 0){
		printf("Error occured during TTF init: %s", SDL_GetError());
		return 0;
	}
	
	bool quit = false; //"LOOPFLAGGA"
	SDL_Event e;
	
	if(SDLNet_Init() < 0){
		printf("Init did not work! Error: %s\n", SDLNet_GetError());
	}

	UDPsocket socket = NULL;
    socket = SDLNet_UDP_Open(0);
    if(socket == NULL){
        printf("Could not create socket! Error: %s\n", SDLNet_GetError());
        return 2;
    }

    IPaddress destip;
    SDLNet_ResolveHost(&destip, "127.0.0.1", 30000);

    UDPpacket *packet = NULL;
    packet = SDLNet_AllocPacket(sizeof(struct TileStruct_t) + 1);
    if(packet == NULL){
        printf("Could not allocate packet! Error: %s\n", SDLNet_GetError());
        return 3;
    }

	IPaddress ipadr;

	sprintf((char *)packet->data, "%s", "connect");    
    packet->address.host = destip.host;	//Destinations host
	packet->address.port = destip.port;	//Destinationens port
	packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(socket, -1, packet);
    printf("Package sent!\n");

	TTF_Font *font = NULL;
	font = TTF_OpenFont("arial.ttf",12);
	if(font == NULL){
		printf("Could not get font!\n");
		return 0;
	}

	Tile *tile = create_Tiles(row,col);
	init_Tiles(tile,row,col,SCREEN_WIDTH,SCREEN_HEIGHT);    
	int mouse_x,mouse_y;
	int buttons;
	int openValue;
	int firstclick = 1;
	int nrOpened = 0;
	Tile_updateAllRenderInfo(tile,renderer,row,col,font);

	while (!quit) {
		Tile tmp;
		recieveUDP(tmp,socket,packet);
		while (SDL_PollEvent(&e) != 0) {
			switch(e.type){
				//Denna ska hoppa över nedanstående och skriva om spelplanen om ett UDP-paket kommer in
				//if(recieveUDP(&tile) == 0){ //Om ett paket kommer in kommer den at automatiskt lägga in info i spelet utan tt vänta på klick från användaren
				case SDL_MOUSEBUTTONDOWN:
					switch(e.button.button){
						case SDL_BUTTON_LEFT:
							buttons = SDL_GetMouseState(&mouse_x,&mouse_y);
							if(firstclick==1){
								handle_TileFirstMouseEvent(tile,mouse_x,mouse_y,row,col,'L',&nrOpened,&firstclick);
								Tile_updateAllRenderInfo(tile,renderer,row,col,font);
								Tile tileptr = get_TileFromMouseClick(tile,mouse_x,mouse_y,row,col);
								sendUDP(tileptr,socket,packet,destip);
							} 
							else{
								quit = handle_TileMouseEvent(tile,mouse_x,mouse_y,row,col,'L',&nrOpened);
								Tile_updateAllRenderInfo(tile,renderer,row,col,font);
								Tile tileptr = get_TileFromMouseClick(tile,mouse_x,mouse_y,row,col);
								sendUDP(tileptr,socket,packet,destip);
							}
							break;
						case SDL_BUTTON_RIGHT:
							buttons = SDL_GetMouseState(&mouse_x,&mouse_y);
							Tile tileptr = get_TileFromMouseClick(tile,mouse_x,mouse_y,row,col);
							if(tileptr != NULL && get_TileState(tileptr) != open){
								change_TileHasFlag(tileptr);													
							}
							sendUDP(tileptr,socket,packet,destip);
							Tile_updateRenderInfo(tileptr,renderer,font);
							break;
						default:
							break;			
					}
					break;
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break;		
			}
		}
		if(nrOpened >= floor(row*col*(1-MAX_NR_OF_MINES_MULTI))){
			quit = true;
		} 	
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/60);
	}
	closeProgram(&window,&renderer);
  	return 0;
}

