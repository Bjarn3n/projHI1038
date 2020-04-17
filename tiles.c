#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "tiles.h"
#include <SDL_Image.h>
#include <SDL.h>
#include <string.h>

void Tile_updateRenderInfo(Tile tile, SDL_Renderer *rend, TTF_Font *font){
    if(tile->state==open){
        if(tile->mine == yes)
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        else{
            SDL_Surface *surf = IMG_Load("openproto.png");
            SDL_Texture *tex = SDL_CreateTextureFromSurface(rend,surf);
            SDL_RenderCopy(rend,tex,NULL,&tile->rect);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
            if((tile->adj) != 0){
                SDL_Color White = {255, 255, 255};
                char buffer[20];
                sprintf(buffer,"%d",(tile->adj));
                SDL_Surface *surf = TTF_RenderText_Solid(font,buffer,White);
                SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);
                if(SDL_RenderCopy(rend, tex, NULL, &tile->textrect) < 0){
                    printf("Problem rendering text", SDL_GetError());
                }
                SDL_FreeSurface(surf);
                SDL_DestroyTexture(tex);                   
            } 
            else{
                SDL_Surface *surf = IMG_Load("openproto.png");
                SDL_Texture *tex = SDL_CreateTextureFromSurface(rend,surf);
                SDL_RenderCopy(rend,tex,NULL,&tile->rect);
                SDL_FreeSurface(surf);
                SDL_DestroyTexture(tex);
            }
        }        
    }
    else{
        SDL_Surface *surf = IMG_Load("closedproto.png");
        SDL_Texture *tex = SDL_CreateTextureFromSurface(rend,surf);
        SDL_RenderCopy(rend,tex,NULL,&tile->rect);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
        if(tile->flag == yesflag){
            SDL_Rect flagga;
            SDL_Texture* image= IMG_LoadTexture(rend,"flag.png");											
            if(SDL_RenderCopy(rend, image, NULL, &tile->rect) < 0)
                printf("Problem rendering text", SDL_GetError());           
            SDL_DestroyTexture(image);    		
        }             	
    }   										
}

void Tile_updateAllRenderInfo(Tile *tile, SDL_Renderer *rend, int r, int c, TTF_Font *font){
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            Tile_updateRenderInfo(&tile[i][j],rend,font);
        }       
    }
}

Tile* create_Tiles(int r, int c){
    Tile *tmp = (Tile*)malloc(r*sizeof(struct TileStruct_t));
    for(int i=0;i<r;i++){
        tmp[i] = malloc(c*sizeof(struct TileStruct_t));
    }
    if(tmp != NULL){
        printf("Memory allocated successfully!\n");
        return (Tile*)tmp;
    }
    else
    {
        printf("Memory couldn't be allocated!\n");
        return NULL;
    }
    
}

void destroy_Tiles(Tile *tile){
    free(tile);
}

int open_Tile(Tile tile, int *nrOpened){
    if(!(get_TileState(tile)) && tile->flag == noflag){
        (*nrOpened)++;
        change_TileState(tile,open);
        if((get_TileHasMine(tile))==yes)
            return -1;
        else{
            return get_TileAdjacent(tile);
        } 
    }
    else return -2;
}

void open_AdjacentTiles(Tile *tile, Tile t_ptr, int r, int c, int *nrOpened){
    int i = t_ptr->pos_y;
    int j = t_ptr->pos_x;
    int openValue;

    for(int y=-1;y<=1;y++){
        for(int x=-1;x<=1;x++){
            if(0 <= i+y && i+y < r && 0 <= j+x && j+x < c){
                openValue = open_Tile(&tile[i+y][j+x],nrOpened);
                if (openValue == 0){
                    open_AdjacentTiles(tile,&tile[i+y][j+x],r,c,nrOpened);
                }
            }
        }
    }
}

void init_Tiles(Tile *tile, int r, int c, int w, int h){
    srand(time(NULL));
    int pixelsize_x = TILE_PIXEL_SIZE;
    int pixelsize_y = TILE_PIXEL_SIZE;
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            set_TilePos(&tile[i][j],'y',i);
            set_TilePos(&tile[i][j],'x',j);
            change_TileHasMine(&tile[i][j],no);             
            set_TileAdjacent(&tile[i][j],0);           
            change_TileState(&tile[i][j],closed);
            tile[i][j].flag = noflag;
            set_TileRect(&tile[i][j],'x',j*pixelsize_x);
            set_TileRect(&tile[i][j],'y',i*pixelsize_y);
            set_TileRect(&tile[i][j],'w',pixelsize_x);
            set_TileRect(&tile[i][j],'h',pixelsize_y);           
            set_TileTextRect(&tile[i][j],'w',pixelsize_x);
            set_TileTextRect(&tile[i][j],'h',pixelsize_y);
            set_TileTextRect(&tile[i][j],'x',j*pixelsize_x);
            set_TileTextRect(&tile[i][j],'y',i*pixelsize_y);
        }       
    }
    printf("Tile initialization successful!\n");

    calc_TileAdjacent(tile,r,c);

    printf("Tiles initialized successfully!\n");
}

void init_TileMines(Tile *tile, Tile firstclick, int r, int c){
    int nrOfMines = 0;
    int maxMines = r*c*MAX_NR_OF_MINES_MULTI;
    for(int i=0;nrOfMines<maxMines;){
        int m1 = rand()%r;
        int m2 = rand()%c;
        if((get_TileHasMine(&tile[m1][m2])) == no && &tile[m1][m2] != firstclick){
            change_TileHasMine(&tile[m1][m2],yes);
            nrOfMines++;
        }
    }
    calc_TileAdjacent(tile,r,c);
}

void set_TileRect(Tile tile, char c, int val){
    switch(c){
        case 'x':
            tile->rect.x = val;
            break;
        case 'y':
            tile->rect.y = val;
            break;
        case 'w':
            tile->rect.w = val;
            break;
        case 'h':
            tile->rect.h = val;
            break;
        default:
            break;
    }
}

void set_TileTextRect(Tile tile, char c, int val){
    switch(c){
        case 'x':
            tile->textrect.x = val + tile->rect.w/4;
            break;
        case 'y':
            tile->textrect.y = val + tile->rect.h/4;
            break;
        case 'w':
            tile->textrect.w = val/2;
            break;
        case 'h':
            tile->textrect.h = val/2;
            break;
        default:
            break;
    }
}

void set_TileAdjacent(Tile tile, int val){
    tile->adj = val;
}

void set_TilePos(Tile tile, char c, int val){
    if(c == 'x'){
        tile->pos_x = val;
    }
    if(c == 'y'){
        tile->pos_y = val;
    }
}

void calc_TileAdjacent(Tile *tile, int r, int c){    
    int adj;    
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            //printf("Going through tile %d, %d \n",tile[i][j].pos_x,tile[i][j].pos_y);
            adj=0;
            for(int y=-1;y<=1;y++){
                for(int x=-1;x<=1;x++){
                    if(0 <= i+y && i+y < r && 0 <= j+x && j+x < c)
                        if(!(y+i == i && x+j == j))
                            if((tile[i+y][j+x].mine) == 1)
                                adj++;
                }
            }
            //printf("Setting new value... %d adjacent\n",adj);
            set_TileAdjacent(&tile[i][j],adj);                            
        }
    }
}

void change_TileHasMine(Tile tile, eMine newmineval){
    tile->mine = newmineval;
}

void change_TileState(Tile tile, eState newstate){
    tile->state = newstate;
}

eState get_TileState(Tile tile){
    return tile->state;
}

eMine get_TileHasMine(Tile tile){
    return tile->mine;
}

int get_TileAdjacent(Tile tile){
    return tile->adj;
}

Tile get_TileFromMouseClick(Tile *tile, int mouse_x, int mouse_y, int r, int c){
    int delta_x = (*tile)->rect.w;
    int delta_y = (*tile)->rect.h;
    for (int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            if(mouse_x >= tile[i][j].rect.x && mouse_x < tile[i][j].rect.x + delta_x)
                if(mouse_y >= tile[i][j].rect.y && mouse_y < tile[i][j].rect.y + delta_y)
                    return &tile[i][j];
        }
    }
    return NULL;
}

void test_PrintAllAdjacent(Tile *tile,int r, int c){
    for (int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            printf("%d,%d ",tile[i][j].mine,tile[i][j].adj);
        }
        printf("\n");
    }

}

char *Tile_TiletoString(Tile tile){
    size_t strlen = 0;
    strlen = snprintf(NULL, strlen, "tile,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",tile->pos_x, tile->pos_y, tile->adj, tile->state, tile->mine, tile->flag, tile->rect.x, tile->rect.y, tile->rect.w, tile->rect.h, tile->textrect.x, tile->textrect.y, tile->textrect.w, tile->textrect.h) + 1;
    char *retstr = calloc(1,sizeof(*retstr)*strlen+1);

    if(snprintf(retstr, strlen, "tile,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",tile->pos_x, tile->pos_y, tile->adj, tile->state, tile->mine, tile->flag, tile->rect.x, tile->rect.y, tile->rect.w, tile->rect.h, tile->textrect.x, tile->textrect.y, tile->textrect.w, tile->textrect.h) > strlen + 1){
        printf("Failed to create string!\n");
        return NULL;
    }
    printf("Tile converted to string!\n");
    return retstr;
}

void Tile_StringtoTile(char *src, Tile dest){
    char *tok = malloc(sizeof(src) + 1);
    int *arr = malloc(sizeof(struct TileStruct_t) + 1);
    long buffer;
    tok = strtok(src,",");
    int i=0;
    while(tok != NULL){
        buffer = strtol(tok,NULL,10);
        arr[i++] = (int) buffer;
        tok = strtok(NULL,",");
    }
    i = 0;
    dest->pos_x = arr[i++];
    dest->pos_y = arr[i++];
    dest->adj = arr[i++];
    dest->state = arr[i++];
    dest->mine = arr[i++];
    dest->flag = arr[i++];
    dest->rect.x = arr[i++];
    dest->rect.y = arr[i++];
    dest->rect.w = arr[i++];
    dest->rect.h = arr[i++];
    dest->textrect.x = arr[i++];
    dest->textrect.y = arr[i++];
    dest->textrect.w = arr[i++];
    dest->textrect.h = arr[i++];
    free(tok);
    free(arr);
    free(src);
}

int handle_TileMouseEvent(Tile *tile, int m_x, int m_y, int r, int c, char m_b, int *nrOpened){    	
    Tile tileptr = get_TileFromMouseClick(tile,m_x,m_y,r,c);
        if(tileptr != NULL && tileptr->flag == noflag){
            int openValue = open_Tile(tileptr,nrOpened);
            if(openValue==0)
                open_AdjacentTiles(tile,tileptr,r,c,nrOpened);
            if(openValue==-1)
                return 1;
        }
    return 0;
}

void handle_TileFirstMouseEvent(Tile *tile, int m_x, int m_y, int r, int c, char m_b, int *nrOpened, int *fc){
    if(m_b == 'L'){
        Tile tileptr = get_TileFromMouseClick(tile,m_x,m_y,r,c);
        if(tileptr != NULL){
            *fc = 0;
            init_TileMines(tile,tileptr,r,c);
            int openValue = open_Tile(tileptr,nrOpened);
            if(openValue == 0)
                open_AdjacentTiles(tile,tileptr,r,c,nrOpened);
        }
    }
}

void change_TileHasFlag(Tile tile){
    if(tile->flag == yesflag){
        tile->flag = noflag;
    } else tile->flag = yesflag;
}