#ifndef MAIN_H
#define MAIN_H

#ifndef __dead
#define __dead __declspec(noreturn)
#endif

static inline bool
strings_match(const char *s1, const char *s2)
{
    return (strcmp(s1, s2) == 0);
}

#endif
