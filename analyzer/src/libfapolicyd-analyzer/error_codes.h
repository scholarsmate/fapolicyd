/*
 * error_codes.h
 */

#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef enum error_code_enum {
  NOT_FOUND = -4,
  RANGE_ERROR = -3,
  PARSE_ERROR = -2,
  ALLOCATION_ERROR = -1,
  OK = 0
} error_code_t;

#endif /* ERROR_CODES_H */
