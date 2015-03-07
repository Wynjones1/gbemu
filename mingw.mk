SRC := $(shell find ./src/ -type f -name *.c)
OBJ := $(patsubst %.c, %.o, $(SRC))
MINGW_PREFIX := i686-w64-mingw32
CC           := $(MINGW_PREFIX)-gcc
CXX          := $(MINGW_PREFIX)-g++
RC           := $(MINGW_PREFIX)-windres)
DEPDIR       := ./deps
INCLUDES     := -I$(DEPDIR)/SDL2/include
INCLUDES     += -I./src
CFLAGS       := --std=c11 -O3
LIBS := -L/home/stuart/C/SDL2-2.0.3/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows

all: $(OBJ)
	$(CC) $^ -o gbemu.exe $(CFLAGS) $(INCLUDES) $(LIBS)

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS) $(INCLUDES) -DSPINLOCK=1

clean:
	find ./src -type f -name "*.o" -exec {} \;

