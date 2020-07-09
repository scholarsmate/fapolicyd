/*
 * macros.h
 */

#define CHECK_PTR(p)                                                           \
  do {                                                                         \
    if (!(p)) {                                                                \
      abort();                                                                 \
    }                                                                          \
  } while (0)
