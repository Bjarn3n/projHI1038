#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_net.h>
#include "networkS.h"
#include "tiles.h"

int recieveUDP(Tile tileRecieve, UDPsocket socket, UDPpacket *package){
    //Kollar om ett paket kommer. Gör det, då kommer värderna sparas och komma till baka till main
	if (SDLNet_UDP_Recv(socket, package))
	{
		printf("UDP package:\n");
		printf("\tChan:    %d\n", package->channel);
		printf("\tData:    %s\n", (char *)package->data);
		printf("\tStatus:  %d\n", package->status);
		printf("\tAddress: %x %x\n", package->address.host, package->address.port);
        Tile_StringtoTile((char *)package->data,tileRecieve);
        printf("Package recieved!\n");
        return 1;
	}
    return 0;
}

int sendUDP(Tile tile, UDPsocket socket, UDPpacket *package, IPaddress ipadr){
    sprintf((char *)package->data, "%s", Tile_TiletoString(tile));    
    package->address.host = ipadr.host;	//Destinations host
	package->address.port = ipadr.port;	//Destinationens port
	package->len = strlen((char *)package->data) + 1;
    SDLNet_UDP_Send(socket, -1, package);
    printf("Package sent!\n");
    return 1;
}

int parseCommand(UDPpacket *p){
    if(strcmp("connect",(char *)p->data) == 0)
        return 1;
    else if(strcmp("quit",(char *)p->data) == 0)
        return -1;
    else return 0;
}

void destroyConnection(Player *player, UDPpacket *p){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(player[i].ipadr.host == p->address.host && player[i].ipadr.port == p->address.port){
            player[i].ipadr.host = 0;
            player[i].ipadr.port = 0;
        }
    }
}

int getPlayerExists(Player *player, UDPpacket *package){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(player[i].ipadr.host == package->address.host && player[i].ipadr.port == package->address.port)
            return 1;
    }
    return 0;
}

void createNewConnection(Player *player, UDPpacket *p){
    for(int i=0;i<MAX_PLAYERS;i++){
        if(player[i].ipadr.host == 0){
            player[i].ipadr.host = p->address.host;
            player[i].ipadr.port = p->address.port;
            printf("New player created!\n");
            return;
        }
    }
}