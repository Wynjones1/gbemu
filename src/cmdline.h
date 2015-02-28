#pragma once
#include "common.h"

//type, short, long, variable, default, description
#define OPTION_LIST\
    X(_Bool  , "v", "verbose"   , verbose   , false                     ,"verbose output.")\
    X(_Bool  , "a", "audio"     , audio     , true                      ,"enable audio")\
    X(_Bool  , "r", "record"    , record    , false                     ,"record input for playback.")\
    X(_Bool  , "p", "replay"    , replay    , false                     ,"replay previously recorded input.");\
    X(string , "i", "in"        , in        , "./data/roms/mario.gb"    ,"rom that will be run.")\
    X(int    , "s", "scale"     , scale     , 3                         ,"scale window size.")\
    X(string , "b", "boot-rom"  , boot_rom  , "./data/boot_roms/DMG.bin","first 256 bytes to be run.")\
    X(string , "" , "break-file", break_file, NULL                      ,"addresses to break on.")

typedef const char * string;

#define X(type, short, long, variable, default, description) type variable;
typedef struct cmdline
{
    OPTION_LIST
}cmdline_t;
#undef X

/* Initialise the cmdline struct */
void cmdline_read(int argc, char **argv);
extern cmdline_t cmdline_args;
