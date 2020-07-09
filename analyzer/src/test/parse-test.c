/*****************************************************************************
 * TESTING
 ****************************************************************************/
#include <libfapolicyd-analyzer/error_codes.h>
#include <libfapolicyd-analyzer/parse.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Emit the parsed record to stdout for testing / debugging
 * 
 * @param parse_ctx_ptr a populated parse_context pointer
 */
void emit_record(const parse_context_t *parse_ctx_ptr, FILE *file_ptr) {
  int emitted_fields = 0;
  field_t field;

  if (OK == get_subject_field(parse_ctx_ptr, &field, emitted_fields)) {
    fprintf(file_ptr, "%s=%s", field.key, field.value);
    while (OK == get_subject_field(parse_ctx_ptr, &field, ++emitted_fields)) {
      fprintf(file_ptr, " %s=%s", field.key, field.value);
    }
  }
  fprintf(file_ptr, " : ");
  emitted_fields = 0;
  if (OK == get_object_field(parse_ctx_ptr, &field, emitted_fields)) {
    fprintf(file_ptr, "%s=%s", field.key, field.value);
    while (OK == get_object_field(parse_ctx_ptr, &field, ++emitted_fields)) {
      fprintf(file_ptr, " %s=%s", field.key, field.value);
    }
  }
  fprintf(file_ptr, "\n");
}

/**
 * @brief Test driver
 * 
 * @param argc count of items in the argument vector
 * @param argv argument vector
 * @return int 0 on success (EXIT_SUCCESS), non-zero on failure (EXIT_FAILURE)
 */
int main(int argc, char *argv[]) {
  FILE *fp = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  parse_context_t *parse_ctx_ptr;

  if (argc == 2) {
    if ((fp = fopen(argv[1], "r")) == NULL) {
      exit(EXIT_FAILURE);
    }
  } else if (argc != 1) {
    fprintf(stderr, "Illegal usage\n");
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&line, &len, fp)) > 0) {
    if (is_audit_record(line)) {
      break;
    }
  }
  if (read < 0) {
    exit(EXIT_FAILURE);
  }
  if (!(parse_ctx_ptr = construct_parse_context())) {
    exit(EXIT_FAILURE);
  }
  if (imbue_parser(parse_ctx_ptr, line) != OK) {
    exit(EXIT_FAILURE);
  }
  do {
    //printf("len: %ld\n", read);
    if (is_audit_record(line)) {
      if (parse(parse_ctx_ptr, line) != OK) {
        exit(EXIT_FAILURE);
      }
      emit_record(parse_ctx_ptr, stdout);
    }
  } while ((read = getline(&line, &len, fp)) > 0);

  fclose(fp);
  free(line);
  destroy_parse_context(parse_ctx_ptr);
  exit(EXIT_SUCCESS);
}
