/*
 * match.h
 */

#ifndef MATCH_H
#define MATCH_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief match a filename pattern against the given string
 * 
 * @param pattern the pattern
 * @param string  the string
 * @return int 0 if the pattern matches the string, and 1 otherwise
 */
int match_filename(const char * pattern, const char * string);


#ifdef __cplusplus
}
#endif

#endif // MATCH_H