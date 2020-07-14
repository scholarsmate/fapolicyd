#include <libfapolicyd-analyzer/rpmdb_read.h>
#include <stdio.h>

int handle_package_info_file(void *clientp, const package_info_t *package_info_ptr, const file_info_t *file_info_ptr) {
  static int count = 0;
  FILE *fp = (FILE *)clientp;
  if (file_info_get_size(file_info_ptr) && S_ISREG(file_info_get_mode(file_info_ptr)) &&
      !file_info_is_config_file(file_info_ptr)) {
    fprintf(fp, "%d\t%s-%s-%s-%s\t%s\t%s\t%lu\t%s\t%s\t%o\t%s\n", ++count, package_info_get_name(package_info_ptr),
            package_info_get_version(package_info_ptr), package_info_get_release(package_info_ptr),
            package_info_get_arch(package_info_ptr), package_info_get_packager(package_info_ptr),
            file_info_get_name(file_info_ptr), file_info_get_size(file_info_ptr), file_info_get_user(file_info_ptr),
            file_info_get_group(file_info_ptr), file_info_get_mode(file_info_ptr),
            file_info_get_sha256_hex(file_info_ptr));
  }
  return 0;
}

int main(int argc, char **argv) { return rpmdb_read(handle_package_info_file, stdout); }