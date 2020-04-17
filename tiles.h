#ifndef TILES_H
#define TILES_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>

// Define default NR of rows
#ifndef NR_OF_ROWS
#define NR_OF_ROWS 10
#endif 

// Define default NR of cols
#ifndef NR_OF_COLS
#define NR_OF_COLS 10
#endif 

// Hur stor andel ska vara minor
#ifndef MAX_NR_OF_MINES_MULTI
#define MAX_NR_OF_MINES_MULTI 0.1
#endif

#ifndef TILE_PIXEL_SIZE
#define TILE_PIXEL_SIZE 32
#endif

/** \brief enum* States som kräves för Tile-structen */
enum tileState{closed,open};
typedef enum tileState eState;
enum hasMine{no,yes};
typedef enum hasMine eMine;
enum hasFlag{noflag,yesflag};
typedef enum hasFlag eFlag;

/* Structen som innehåller alla data som är relevant för de tiles som skall användas
    pos_x, pos_y: Positioner relativt andra tiles. Från [0,0] -> [r-1,c-1] där 
    r = rader, och c = kolumner
    adj: hur många minor som finns "adjacent", alltså de 8 rutorna runtomkring, tillåtna värden är 0-8
    rect: definierar en rektangel som används för att rendera tiles på skärmen
    state: vilken state som tilen har - open, closed, flag
    mine: har tilen en mina eller inte  

    inga värden bör ändras utan att kalla på de funktioner som existerar i tiles.c
*/
struct TileStruct_t{
    int pos_x,pos_y;
    int adj;
    SDL_Rect rect;
    SDL_Rect textrect;
    eState state;
    eMine mine;
    eFlag flag;
};
typedef struct TileStruct_t *Tile;

struct TileRender_t{
    eState state;
    eFlag flag;
    eMine mine;
};
typedef struct TileRender_t *TileRender;

/* Returnerar en pointer till en Struct Pointer av typen Tile (Tile är en struct TileStruct_t *) med r Rader och c Kolumner.
 int r är mängder rader - endast heltal
 int c är mängder kolumner - endast heltal
 Pointern är ekvivalent till en 2d array */
Tile *create_Tiles(int r, int c);
/* Returnerar minnet som användes för att skapa structen */
void destroy_Tiles(Tile *tile); 


// -----------------------------------------------------------------------------
/* Init + Operators
 "Privata funktioner" - kör bara dessa från tiles.c */

// Kör denna först för att initialisera alla värden 
void init_Tiles(Tile *tile, int r, int c, int w, int h);
void init_TileMines(Tile *tile, Tile firstclick, int r, int c);
// Borde bara andropas vid skapande av tiles
void set_TilePos(Tile tile, char c, int val);
// Borde bara andropas vid skapande av tiles
void set_TileRect(Tile tile, char c, int val);
void set_TileTextRect(Tile tile, char c, int val);
// Borde bara andropas vid skapande av tiles
// Använder adjacent tiles mine värden för att räkna ut hur många minor som ligger adjacent
void calc_TileAdjacent(Tile *tile, int r, int c);
void set_TileAdjacent(Tile tile, int val);

/* ------------------------------ Ändra bara värden genom dessa --------------------------- */
void change_TileHasMine(Tile tile, eMine newmineval);
void change_TileState(Tile tile, eState newstate);

/*----------------------------------------------------------------------------------
 "Public" Funktioner - dessa kan köras varsomhelst
  ----------------------------------------------------------------------------------*/
// Öppnar tile om den är stängd
int open_Tile(Tile tile, int *nrOpened);
void open_AdjacentTiles(Tile *tile, Tile t_ptr, int r, int c, int *nrOpened);
// Rita enskild Tile
void Tile_updateRenderInfo(Tile tile, SDL_Renderer *rend, TTF_Font *font); 
// Rita alla Tiles
void Tile_updateAllRenderInfo(Tile *tile, SDL_Renderer *rend, int r, int c, TTF_Font *font); 
// Returnera vilken Tile som musens position kolliderar med
Tile get_TileFromMouseClick(Tile *tile, int mouse_x, int mouse_y, int r, int c);
eState get_TileState(Tile tile);
eMine get_TileHasMine(Tile tile);
int get_TileAdjacent(Tile tile);

void Tile_StringtoTile(char * src, Tile dest);
char *Tile_TiletoString(Tile tile);
int handle_TileMouseEvent(Tile *tile, int m_x, int m_y, int r, int c, char m_b, int *nrOpened);
void handle_TileFirstMouseEvent(Tile *tile, int m_x, int m_y, int r, int c, char m_b, int *nrOpened, int *fc);

/* Debug funktioner */
void test_PrintAllAdjacent(Tile *tile,int r, int c);
void change_TileHasFlag(Tile tile);

#endif