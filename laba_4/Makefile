MATH_FLAG = -lm
PIC_FLAG = -fPIC
SHARED_FLAG = -shared
DYNAMIC_LOADING_FLAG = -ldl
EXT = c
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall

# all: linking_realization1 linking_realization2 dynamic
all:  stat_main dynamic

# linking_realization2: stat_main dynamic_realization2
# 	$(CC) $(CFLAGS) stat_main.o -L. -lsecond_realization -o linking_realization2 $(MATH_FLAG)


# linking_realization1: stat_main dynamic_realization1
# 	$(CC) $(CFLAGS) stat_main.o -L. -lfirst_realization -o linking_realization1 $(MATH_FLAG)

stat_main: stat_main.$(EXT) first_realization.$(EXT) function.h
	$(CC) $(CFLAGS) stat_main.$(EXT) first_realization.$(EXT) -o stat_main $(MATH_FLAG)

dynamic: dyn_main dynamic_realization1 dynamic_realization2
	$(CC) $(CFLAGS) dyn_main.o -o dynamic $(DYNAMIC_LOADING_FLAG)

dyn_main : dyn_main.$(EXT)
	$(CC) $(CFLAGS) -c dyn_main.$(EXT)

dynamic_realization2: second_realization.$(EXT) function.h
	$(CC) $(CFLAGS) $(PIC_FLAG) $(SHARED_FLAG) -o libsecond_realization.so second_realization.$(EXT) $(MATH_FLAG)

dynamic_realization1: first_realization.$(EXT) function.h
	$(CC) $(CFLAGS) $(PIC_FLAG) $(SHARED_FLAG) -o libfirst_realization.so first_realization.$(EXT) $(MATH_FLAG)

clean :
	rm *.o *.so  stat_main dynamic