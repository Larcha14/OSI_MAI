TARGET_1 = main
TARGET_2 = child
EXT = c
CC = gcc
FUNCT = function
MAP = m_map
CFLAGS = -std=c99 -pedantic -Wall
FTRUNCATE = -D_XOPEN_SOURCE=500

$(TARGET_1) : $(TARGET_2) $(FUNCT).$(EXT) $(MAP).$(EXT) $(TARGET_1).$(EXT) 
	$(CC) $(CFLAGS) $(FTRUNCATE) $(TARGET_1).$(EXT) $(FUNCT).$(EXT) $(MAP).$(EXT) -o $(TARGET_1)

$(TARGET_2) : $(FUNCT).$(EXT) $(MAP).$(EXT) $(TARGET_2).$(EXT)
	$(CC) $(CFLAGS) $(FTRUNCATE) $(TARGET_2).$(EXT) $(FUNCT).$(EXT) $(MAP).$(EXT) -o $(TARGET_2)

clean :
	rm $(TARGET_1) $(TARGET_2) *.o