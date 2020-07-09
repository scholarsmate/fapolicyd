/*****************************************************************************
 * TESTING
 ****************************************************************************/
#include <libfapolicyd-analyzer/match.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Test function for match_filename
 *
 * @param pattern the pattern
 * @param string the string
 * @param should_match if the pattern is expected to match the string
 * @return int 0 if the test passes and 1 if the test failed
 */
int test(const char *pattern, const char *string, bool should_match) {
  return ((match_filename(pattern, string) == 0) == should_match) ? 0 : 1;
}

/**
 * @brief Test driver
 *
 * @return int
 */
int main(void) {
  int failures = 0;

  if (0 != test("/etc/passwd", "/etc/passwd", true)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }
  if (0 != test("/etc/*", "/etc/passwd", true)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }
  if (0 != test("/etc/passw*", "/etc/passwd", true)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }
  if (0 != test("/etc/passw?", "/etc/passwd", true)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }
  if (0 != test("/etc/p?ssw?", "/etc/passwd", true)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }
  if (0 != test("/etc/p*d", "/etc/passwd", true)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }
  if (0 != test("/etc/group", "/etc/passwd", false)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }
  if (0 != test("/etc/*p", "/etc/passwd", false)) {
    puts("FAILED");
    ++failures;
  } else {
    puts("PASS");
  }

  exit((failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE));
}
