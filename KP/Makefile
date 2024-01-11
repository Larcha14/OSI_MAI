SERVER = server
CLIENT = client
CC = g++
EXT = cpp
CFLAGS = -pedantic -Wall

all : $(SERVER) $(CLIENT)

$(SERVER) : $(SERVER).$(EXT)  m_map.$(EXT)
	$(CC) $(OBJ) m_map.$(EXT)  $(SERVER).$(EXT) -o $(SERVER) $(FLAG)

$(CLIENT) : $(CLIENT).$(EXT) m_map.$(EXT)
	$(CC) $(OBJ) m_map.$(EXT) $(CLIENT).$(EXT)  -o $(CLIENT) $(FLAG)

clean : 
	rm $(SERVER) $(CLIENT)  

