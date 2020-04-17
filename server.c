#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include "tiles.h"
#include "networkS.h"
#include "spelplan.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char **args){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Could not initialize SDL! Error: %s\n", SDL_GetError());
        return 1;
    }
    if(SDLNet_Init() < 0){
        printf("Could not initialize SDLNet! Error: %s\n", SDLNet_GetError());
        return 1;
    }

    UDPsocket socket = NULL;
    socket = SDLNet_UDP_Open(30000);
    if(socket == NULL){
        printf("Could not create socket! Error: %s\n", SDLNet_GetError());
        return 2;
    }

    IPaddress serverip;
    SDLNet_ResolveHost(&serverip, NULL, 30000);

    UDPpacket *packet = NULL;
    packet = SDLNet_AllocPacket(sizeof(struct TileStruct_t) + 1);
    if(packet == NULL){
        printf("Could not allocate packet! Error: %s\n", SDLNet_GetError());
        return 3;
    }
    Tile *tilearray;
    Tile tile = malloc(sizeof(struct TileStruct_t) + 1);
    int quit = 0;
    Player player[MAX_PLAYERS] = {
        { "", 0, 0},
        { "", 0, 0},
        { "", 0, 0},
        { "", 0, 0},
    };

    while(!quit){
        if(recieveUDP(tile,socket,packet)){
            if(parseCommand(packet) == 1){
                for(int i=0;i<MAX_PLAYERS;i++){
                    if(getPlayerExists(player,packet) == 0)
                        createNewConnection(player,packet);
                }                    
            }
            else if(parseCommand(packet) == 0){
                for(int i=0;i<MAX_PLAYERS;i++){
                    if(getPlayerExists(player,packet) == 1)
                        sendUDP(tile,socket,packet,player[i].ipadr);
                }
            }
            else if(parseCommand(packet) == -1){
                destroyConnection(player,packet);
            }
        }
    }
    SDLNet_UDP_Close(socket);
    SDLNet_Quit();
    SDL_Quit();           
    return 0;
}
 
  