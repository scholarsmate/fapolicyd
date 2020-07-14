#ifndef RPMDB_READ_H
#define RPMDB_READ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/stat.h>

typedef struct package_info_struct package_info_t;
const char *package_info_get_name(const package_info_t *package_info_ptr);
const char *package_info_get_version(const package_info_t *package_info_ptr);
const char *package_info_get_release(const package_info_t *package_info_ptr);
const char *package_info_get_arch(const package_info_t *package_info_ptr);
const char *package_info_get_packager(const package_info_t *package_info_ptr);

typedef struct file_info_struct file_info_t;
const char *file_info_get_name(const file_info_t *file_info_ptr);
const char *file_info_get_user(const file_info_t *file_info_ptr);
const char *file_info_get_group(const file_info_t *file_info_ptr);
const char *file_info_get_sha256_hex(const file_info_t *file_info_ptr);
uint64_t file_info_get_size(const file_info_t *file_info_ptr);
mode_t file_info_get_mode(const file_info_t *file_info_ptr);
int file_info_is_config_file(const file_info_t *file_info_ptr);

typedef int (*package_file_callback_t)(void *, const package_info_t *, const file_info_t *);
int rpmdb_read(package_file_callback_t callback, void *clientp);

#ifdef __cplusplus
}
#endif

#endif /* RPMDB_READ_H */
