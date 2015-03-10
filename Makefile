SRCDIR := ./src
OBJDIR := ./obj
SRC    := $(shell find $(SRCDIR) -type f -name "*.c")
OBJ    := $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))
LIBS   := 
CC     := clang++

CFLAGS := --std=c++11
CFLAGS += -Wno-enum-conversion
CFLAGS += -I$(SRCDIR)
CFLAGS += -I$(shell sdl2-config --cflags)
CFLAGS += -O3
LIBS   += $(shell sdl2-config --libs)
LIBS   += -lSDL2_ttf

all: build

build: $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o gbemu

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -Rf $(OBJDIR)

run: all
	./gbemu
