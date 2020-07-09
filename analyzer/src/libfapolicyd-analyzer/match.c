/*
 * match.c
 */

#include "match.h"
#include <fnmatch.h>

int match_filename(const char *pattern, const char *string) {
  static const int flags = FNM_PATHNAME | FNM_PERIOD;
  return fnmatch(pattern, string, flags);
}
