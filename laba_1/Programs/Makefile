TARGET_1 = main
TARGET_2 = child
EXT = c
CC = gcc
FUNCT = function
CFLAGS = -std=c99 -pedantic -Wall

$(TARGET_1) : $(TARGET_2) $(FUNCT).$(EXT) $(TARGET_1).$(EXT) 
	$(CC) $(CFLAGS) $(TARGET_1).$(EXT) $(FUNCT).$(EXT) -o $(TARGET_1)

$(TARGET_2) : $(FUNCT).$(EXT) $(TARGET_2).$(EXT)
	$(CC) $(CFLAGS) $(TARGET_2).$(EXT) $(FUNCT).$(EXT) -o $(TARGET_2)

clean :
	rm $(TARGET_1) $(TARGET_2) *.o