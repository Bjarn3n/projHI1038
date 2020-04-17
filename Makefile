OBJS = main.o tiles.o spelplan.o networkS.o
SOBJS = server.o tiles.o spelplan.o networkS.o
CC = gcc
OBJ_NAME = main.exe
SOBJ_NAME = server.exe
# path till alla SDL libraries
LIB_PATH = -LC:\MinGW\lib
# path till alla .h filer från SDL, ".\" betyder utgå från samma folder som du kör Makefilen i
INC_PATH = -IC:\MinGW\include

all: main.exe server.exe

main.exe: $(OBJS)
	gcc $(OBJS) $(INC_PATH) $(LIB_PATH) -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -o $(OBJ_NAME)
server.exe: $(SOBJS)
	gcc $(SOBJS) $(INC_PATH) $(LIB_PATH) -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -o $(SOBJ_NAME)

main.o: main.c
	gcc -c main.c $(INC_PATH)	
tiles.o: tiles.c tiles.h
	gcc -c tiles.c $(INC_PATH)
spelplan.o: spelplan.c spelplan.h
	gcc -c spelplan.c $(INC_PATH)
networkS.o: networkS.c networkS.h
	gcc -c networkS.c $(INC_PATH)
server.o: server.c
	gcc -c server.c -o server.o

.PHONY: clean
clean:
	del *.o
	del *.exe