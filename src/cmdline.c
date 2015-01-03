#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cmdline.h"
#include "common.h"

#define SET_DEFAULT (i == -1)
#define HAVE_NEXT   (i + 1 < argc)
#define NEXT        argv[i + 1]
#define CMP(s, l, str)\
	(strcmp("-" s, str) == 0 || strcmp("--" l, str) == 0)
#define OPTION(s, l) CMP(s, l, argv[i])
#define ARGS cmdline_args
#define OPTION_INT(s, l, name, default_val, description)        \
    if(PRINT_HELP)                                              \
    {                                                           \
        printf("-%s/--%-10s: %s (default : %d)\n",              \
            s,l, description, default_val);                     \
    }                                                           \
    else if(SET_DEFAULT)                                        \
    {                                                           \
        ARGS.name = default_val;                                 \
    }                                                           \
    else if(OPTION(s, l))                                       \
    {                                                           \
        if(HAVE_NEXT)                                           \
        {                                                       \
            ARGS.name = atoi(NEXT);                              \
            i++;                                                \
            continue;                                           \
        }                                                       \
        else Error("Must supply argument for --%s/-%s\n", l, s);\
    }
#define OPTION_BOOL(s, l, name, description)                    \
    if(PRINT_HELP)                                              \
    {                                                           \
        printf("-%s/--%-10s: %s\n", s,l, description);          \
    }                                                           \
    else if(!SET_DEFAULT && OPTION(s, l))                       \
    {                                                           \
        ARGS.name = true;                                        \
        continue;                                               \
    }
#define OPTION_STRING(s, l, name, default_val, description)     \
    if(PRINT_HELP)                                              \
    {                                                           \
        if(default_val)                                         \
        {                                                       \
            printf("-%s/--%-10s: %s (default : %s)\n",          \
                s,l, description, default_val ? default_val : ""); \
        }                                                       \
        else                                                    \
        {                                                       \
            printf("-%s/--%-10s: %s\n", s,l, description);      \
        }                                                       \
    }                                                           \
    else if(SET_DEFAULT)                                        \
    {                                                           \
        ARGS.name = default_val;                                 \
    }                                                           \
    else if(OPTION(s, l))                                       \
    {                                                           \
        if(HAVE_NEXT)                                           \
        {                                                       \
            ARGS.name = NEXT;                                    \
            i++;                                                \
            continue;                                           \
        }                                                       \
        else Error("Must supply argument for --%s/-%s\n", l, s);\
    }

#define OPTION_HELP()\
    if(PRINT_HELP)\
    {\
        exit(0);\
    }\
    else if(SET_DEFAULT)\
    {}\
    else if(OPTION("h", "help"))\
    {\
        PRINT_HELP = true;\
        goto print_help_label;\
    }

void cmdline_read(int argc, char **argv)
{
    memset(&cmdline_args, 0x00, sizeof(cmdline_args));
    bool PRINT_HELP= false;
	for(int i = -1; i < argc; i++)
	{
print_help_label:
        OPTION_BOOL(  "v", "verbose",   verbose,                             "verbose output.");
        OPTION_BOOL(  "a", "audio",     audio,                               "enable audio");
        OPTION_BOOL(  "r", "record",    record,                              "record input for playback.");
        OPTION_BOOL(  "p", "replay",    replay,                              "replay previously recorded input."); 
        OPTION_STRING("i", "in",        in,        "./data/roms/pokemon_blue.gb",   "rom that will be run.");
        OPTION_INT(   "s", "scale",     scale,     3,                        "scale window size.");
        OPTION_STRING("b", "boot-rom",  boot_rom, "./data/boot_roms/DMG.bin","first 256 bytes to be run.");
        OPTION_STRING("", "break-file", break_file, NULL, "addresses to break on.");
        OPTION_HELP();
        if(i > 1)
            Error("Unrecognised option %s\n", argv[i]);
	}
}

cmdline_t cmdline_args;
