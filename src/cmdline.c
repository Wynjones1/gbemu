#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cmdline.h"
#include "logging.h"
#include "common.h"

#define SET_DEFAULT (i == -1)
#define HAVE_NEXT   (i + 1 < argc)
#define NEXT        argv[i + 1]
#define CMP(s, l, str)\
	(strcmp("-" s, str) == 0 || strcmp("--" l, str) == 0)
#define OPTION(s, l) CMP(s, l, argv[i])
#define ARGS cmdline_args
#define OPTION_int(s, l, name, default_val, description)        \
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
#define OPTION_bool(s, l, name, _, description)                    \
    if(PRINT_HELP)                                              \
    {                                                           \
        printf("-%s/--%-10s: %s\n", s,l, description);          \
    }                                                           \
    else if(!SET_DEFAULT && OPTION(s, l))                       \
    {                                                           \
        ARGS.name = true;                                        \
        continue;                                               \
    }
#define OPTION_string(s, l, name, default_val, description)     \
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
        #define X(type, short, long, var, default, desc) OPTION_ ## type(short, long, var, default, desc);
        OPTION_LIST
        #undef X
        OPTION_HELP();
        if(i > 1) log_error("Unrecognised option %s\n", argv[i]);
	}
}

cmdline_t cmdline_args;
