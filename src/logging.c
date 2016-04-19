#include "logging.h"
#include "common.h"
#include "platform.h"
#include <stdarg.h>

void log_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);
#if PLATFORM_SDL
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);
#endif
    va_end(args);
}

void log_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
#if PLATFORM_SDL
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);
#endif
    va_end(args);
}

void log_warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);
#if PLATFORM_SDL
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);
#endif
    va_end(args);
}

void log_verbose(const char *format, ...)
{
#if VERBOSE
    va_list args;
    va_start(args, format);
#if PLATFORM_SDL
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);
#endif
    va_end(args);
#endif
}
