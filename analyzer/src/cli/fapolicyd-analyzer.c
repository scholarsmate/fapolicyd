/*
allow perm=execute auid=1001 exe=/… trust=1 : path=/… trust=1
allow perm=open auid=1001 exe=/… trust=1 : path=/… trust=1
deny_audit perm=execute auid=1001 exe=/usr/libexec/platform-python3.6 : all
*/
#include <libfapolicyd-analyzer/error_codes.h>
#include <libfapolicyd-analyzer/util/field_array.h>
#include <libfapolicyd-analyzer/util/hash_set.h>
#include <libfapolicyd-analyzer/parse.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Holds sets of allow rules and deny rules.
 * The assumption is that all allow rules and deny rules can be unorderd,
 * but that all allow rules come before any deny rules.  It is likely that
 * there will be only one deny rule and it will deny anything not explictly
 * allowed.
 */
typedef struct rules_struct {
  hash_set_t *allow_rules;
  hash_set_t *deny_rules;
} rules_t;

hash_set_t *get_allow_rules(rules_t *rules_ptr) { return rules_ptr->allow_rules; }

void set_allow_rules(rules_t *rules_ptr, hash_set_t *hash_set_ptr) { rules_ptr->allow_rules = hash_set_ptr; }

hash_set_t *get_deny_rules(rules_t *rules_ptr) { return rules_ptr->deny_rules; }

void set_deny_rules(rules_t *rules_ptr, hash_set_t *hash_set_ptr) { rules_ptr->deny_rules = hash_set_ptr; }

rules_t *rules_construct(void) {
  rules_t *rules_ptr = malloc(sizeof(rules_t));
  if (rules_ptr) {
    set_allow_rules(rules_ptr, hash_set_construct());
    set_deny_rules(rules_ptr, hash_set_construct());
    hash_set_initialize(get_allow_rules(rules_ptr), 0);
    hash_set_initialize(get_deny_rules(rules_ptr), 0);
  }
  return rules_ptr;
}

void rules_destroy(rules_t *rules_ptr) {
  hash_set_destroy(get_allow_rules(rules_ptr));
  hash_set_destroy(get_deny_rules(rules_ptr));
  free(rules_ptr);
}

const char *get_subject_value(parse_context_t *parse_ctx_ptr, const char *key) {
  field_t field;
  size_t field_number = 0;
  while (OK == get_subject_field(parse_ctx_ptr, &field, field_number++)) {
    if (0 == strcmp(field.key, key)) {
      return field.value;
    }
  }
  return NULL;
}

const char *get_object_value(parse_context_t *parse_ctx_ptr, const char *key) {
  field_t field;
  size_t field_number = 0;
  while (OK == get_object_field(parse_ctx_ptr, &field, field_number++)) {
    if (0 == strcmp(field.key, key)) {
      return field.value;
    }
  }
  return NULL;
}

int rule_create(char *buf, size_t buf_len, const char *dec, const char *perm, uint32_t uid, const char *exe,
                bool trust_subject, const char *path, bool trust_object) {
  const char *sub_trust = (trust_subject) ? " trust=1" : "";
  const char *obj_trust = (trust_object) ? " trust=1" : "";
  return snprintf(buf, buf_len - 1, "%s perm=%s auid=%u exe=%s%s : path=%s%s", dec, perm, uid, exe, sub_trust, path,
                  obj_trust);
}

void process_record(parse_context_t *parse_ctx_ptr, rules_t *rules_ptr) {
  uint32_t uid = 1001;
  bool trust_subject = true;
  const char *dec = get_subject_value(parse_ctx_ptr, "dec");
  const char *perm = get_subject_value(parse_ctx_ptr, "perm");
  const char *exe = get_subject_value(parse_ctx_ptr, "exe");

  bool trust_object = true;
  const char *path = get_object_value(parse_ctx_ptr, "path");
  char rule[2048];
  rule_create(rule, sizeof(rule), dec, perm, uid, exe, trust_subject, path, trust_object);
  hash_set_add(get_allow_rules(rules_ptr), rule);
}

void emit_rules(rules_t *rules_ptr, FILE *fp) {
  size_t index = 0;
  const char *key;
  hash_set_t *rules = get_allow_rules(rules_ptr);
  while ((key = hash_set_next(rules, &index))) {
    fprintf(fp, "%s\n", key);
  }
  rules = get_deny_rules(rules_ptr);
  while ((key = hash_set_next(rules, &index))) {
    fprintf(fp, "%s\n", key);
  }
}

int main(int argc, char *argv[]) {
  FILE *fp = stdin;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  parse_context_t *parse_ctx_ptr;
  rules_t *rules_ptr;

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
  rules_ptr = rules_construct();
  do {
    // printf("len: %ld\n", read);
    if (is_audit_record(line)) {
      if (parse(parse_ctx_ptr, line) != OK) {
        exit(EXIT_FAILURE);
      }
      process_record(parse_ctx_ptr, rules_ptr);
    }
  } while ((read = getline(&line, &len, fp)) > 0);

  emit_rules(rules_ptr, stdout);

  rules_destroy(rules_ptr);
  fclose(fp);
  free(line);
  destroy_parse_context(parse_ctx_ptr);
  exit(EXIT_SUCCESS);
}
