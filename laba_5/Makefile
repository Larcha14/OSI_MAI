MANAGE_MAIN = manage_main
NODE = node
CC = g++
EXT = cpp
FLAG = -lzmq
CFLAGS = -pedantic -Wall

all : $(MANAGE_MAIN) $(NODE)

$(MANAGE_MAIN) : $(MANAGE_MAIN).$(EXT) awlTree.$(EXT) manage_node.$(EXT) timer.$(EXT)
	$(CC) $(OBJ) awlTree.$(EXT) manage_node.$(EXT) timer.$(EXT) $(MANAGE_MAIN).$(EXT) -o $(MANAGE_MAIN) $(FLAG)

$(NODE) : $(NODE).$(EXT) timer.$(EXT)
	$(CC) $(OBJ) $(NODE).$(EXT) timer.$(EXT) -o $(NODE) $(FLAG)

clean : 
	rm $(MANAGE_MAIN) $(NODE) *.o 

