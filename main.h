#ifndef MAIN_H
#define MAIN_H

#include <ctype.h>
#include <string.h>

#ifndef __dead
#define __dead __declspec(noreturn)
#endif

__dead void fatal(const char *);

static inline void
advance_while_isspace(const char **cp)
{
    while (isspace(**cp))
	(*cp) ++;
}

static inline bool
strings_match(const char *s1, const char *s2)
{
    return (strcmp(s1, s2) == 0);
}

#endif
