/**
* @file error.c
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief Error handling and exitting with specified exit code and error message
* @author Jakub Dufek (x274855)
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "error.h"


void error_exit(int err_code, unsigned line, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "Error %d", err_code);
    // no line num needed with specific error cases
    if (line != 0) {
        fprintf(stderr, " on line %u", line);
    }
    fprintf(stderr, ": ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(err_code);
}
