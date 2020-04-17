//Storlek reserverat för paket
#ifndef PKTSZ
#define PKTSZ 512
#endif 

#ifndef MAX_PLAYERS
#define MAX_PLAYERS 4
#endif

#ifndef _NETWORKS_H
#define _NETWORKS_H

#include <SDL_net.h>
#include "tiles.h"

struct player{
    char name[20];
    IPaddress ipadr;
    Tile *player_tile;
}; typedef struct player Player;

int recieveUDP(Tile tileRecieve, UDPsocket socket, UDPpacket *package);
int sendUDP(Tile tile, UDPsocket socket, UDPpacket *package, IPaddress ipadr);
int getPlayerExists(Player *player, UDPpacket *package);
int parseCommand(UDPpacket *p);
void createNewConnection(Player *player, UDPpacket *p);
void destroyConnection(Player *player, UDPpacket *p);

#endif