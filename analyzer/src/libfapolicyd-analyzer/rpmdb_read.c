#include "rpmdb_read.h"
#include <rpm/rpmdb.h>
#include <rpm/rpmlib.h>
#include <rpm/rpmmacro.h>
#include <rpm/rpmts.h>

struct package_info_struct {
  Header package_header;
};

struct file_info_struct {
  rpmfi file_info;
  const char *file_sha256_hex;
};

const char *package_info_get_name(const package_info_t *package_info_ptr) {
  return headerGetString(package_info_ptr->package_header, RPMTAG_NAME);
}
const char *package_info_get_version(const package_info_t *package_info_ptr) {
  return headerGetString(package_info_ptr->package_header, RPMTAG_VERSION);
}
const char *package_info_get_release(const package_info_t *package_info_ptr) {
  return headerGetString(package_info_ptr->package_header, RPMTAG_RELEASE);
}
const char *package_info_get_arch(const package_info_t *package_info_ptr) {
  return headerGetString(package_info_ptr->package_header, RPMTAG_ARCH);
}
const char *package_info_get_packager(const package_info_t *package_info_ptr) {
  return headerGetString(package_info_ptr->package_header, RPMTAG_PACKAGER);
}

const char *file_info_get_name(const file_info_t *file_info_ptr) { return rpmfiFN(file_info_ptr->file_info); }
const char *file_info_get_user(const file_info_t *file_info_ptr) { return rpmfiFUser(file_info_ptr->file_info); }
const char *file_info_get_group(const file_info_t *file_info_ptr) { return rpmfiFGroup(file_info_ptr->file_info); }
const char *file_info_get_sha256_hex(const file_info_t *file_info_ptr) { return file_info_ptr->file_sha256_hex; }
uint64_t file_info_get_size(const file_info_t *file_info_ptr) { return rpmfiFSize(file_info_ptr->file_info); }
mode_t file_info_get_mode(const file_info_t *file_info_ptr) { return rpmfiFMode(file_info_ptr->file_info); }
int file_info_is_config_file(const file_info_t *file_info_ptr) {
  return (rpmfiFFlags(file_info_ptr->file_info) & (RPMFILE_CONFIG | RPMFILE_MISSINGOK | RPMFILE_NOREPLACE)) ? 1 : 0;
}

int rpmdb_read(package_file_callback_t callback, void *clientp) {
  package_info_t package_info;
  file_info_t file_info;
  int callback_rc = 0;
  rpmReadConfigFiles(NULL, NULL);
  rpmts ts = rpmtsCreate();

  rpmdbMatchIterator mi = rpmtsInitIterator(ts, RPMDBI_PACKAGES, NULL, 0);
  while (callback_rc == 0 && (package_info.package_header = rpmdbNextIterator(mi)) != NULL) {
    file_info.file_info = rpmfiNew(NULL, package_info.package_header, RPMTAG_BASENAMES, RPMFI_KEEPHEADER);
    while (callback_rc == 0 && rpmfiNext(file_info.file_info) != -1) {
      // rpmfiFDigestHex allocates and transfers ownership, so manage the pointer
      file_info.file_sha256_hex = rpmfiFDigestHex(file_info.file_info, NULL);
      callback_rc = callback(clientp, &package_info, &file_info);
      free((char *)file_info.file_sha256_hex);
    }
    rpmfiFree(file_info.file_info);
  }
  headerFree(package_info.package_header);
  rpmdbFreeIterator(mi);
  rpmtsFree(ts);
  rpmFreeCrypto();
  rpmFreeRpmrc();
  rpmFreeMacros(NULL);
  return 0;
}
