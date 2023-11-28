TARGET = lab_2
CC = g++
CFLAGS = -pedantic -Wall

SCR = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SCR))
HEADERS = $(wildcard ./include/*.h)

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)


clean : 
	rm $(TARGET) *.o 
