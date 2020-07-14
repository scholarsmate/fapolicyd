#include "rpmdb_table.hpp"
#include "error_codes.h"
#include "rpmdb_read.h"
#include "sha256.h"
#include <boost/lexical_cast.hpp>
#include <sstream>

#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

static inline const char *gid_as_group(gid_t gid) {
  struct group *grp = getgrgid(gid);
  return (grp) ? grp->gr_name : NULL;
}
static inline const char *uid_as_user(uid_t uid) {
  struct passwd *pws = getpwuid(uid);
  return (pws) ? pws->pw_name : NULL;
}
static inline const char *mode_as_string(mode_t mode) {
  static char buf[16];
  snprintf(buf, sizeof(buf), "%o", mode);
  return buf;
}

int package_file_callback(void *clientp, const package_info_t *package_info_ptr, const file_info_t *file_info_ptr) {
  if (file_info_get_size(file_info_ptr) && S_ISREG(file_info_get_mode(file_info_ptr))) {
    table_t *table_ptr = (table_t *)clientp;
    std::ostringstream nvra_oss;
    vect_of_strings_t record;

    record.reserve(table_ptr->get_column_count());
    record.push_back(std::string()); /* reserve for NVRA */
    record.push_back(package_info_get_name(package_info_ptr));
    record.push_back(package_info_get_version(package_info_ptr));
    record.push_back(package_info_get_release(package_info_ptr));
    record.push_back(package_info_get_arch(package_info_ptr));
    record.push_back(package_info_get_packager(package_info_ptr));
    record.push_back(file_info_get_name(file_info_ptr));
    record.push_back(file_info_get_user(file_info_ptr));
    record.push_back(file_info_get_group(file_info_ptr));
    record.push_back(mode_as_string(file_info_get_mode(file_info_ptr)));
    record.push_back(boost::lexical_cast<std::string>(file_info_get_size(file_info_ptr)));
    record.push_back(file_info_get_sha256_hex(file_info_ptr));
    record.push_back((file_info_is_config_file(file_info_ptr)) ? "1" : "0");
    nvra_oss << record[1] << "-" << record[2] << "-" << record[3] << "-" << record[4];
    record[0] = nvra_oss.str();
    table_ptr->push_back_row(record);
  }
  return 0;
}

table_t *rpmdb_read_as_table() {
  table_t *table_ptr(new table_t());

  table_ptr->initialize({"PACKAGE_NVRA", "PACKAGE_NAME", "PACAKGE_VERSION", "PACKAGE_REVISION", "PACKAGE_ARCH",
                         "PACKAGE_PACKAGER", "FILE_NAME", "FILE_INITIAL_USER", "FILE_INITIAL_GROUP",
                         "FILE_INITIAL_MODE", "FILE_INITIAL_SIZE", "FILE_INITIAL_SHA256", "FILE_IS_CONFIG"});
  rpmdb_read(package_file_callback, table_ptr);
  return table_ptr;
}

table_t *rpmdb_hash_files(const table_t *rpmdb_table_ptr, package_progress_callback_t progress_callback,
                          void *progress_client_ptr) {
  const int row_count = rpmdb_table_ptr->get_row_count();
  const int nvra_col_num = rpmdb_table_ptr->find_column_number("PACKAGE_NVRA");
  const int file_path_col_num = rpmdb_table_ptr->find_column_number("FILE_NAME");
  char hash[65];
  table_t *table_ptr(new table_t());

  if (row_count < 0 || file_path_col_num < 0 || nvra_col_num < 0) {
    abort();
  }
  table_ptr->initialize({"PACKAGE_ROW_NUM", "PACKAGE_NVRA", "FILE_NAME", "FILE_CURRENT_USER", "FILE_CURRENT_GROUP",
                         "FILE_CURRENT_MODE", "FILE_CURRENT_SIZE", "FILE_CURRENT_SHA256"});
  for (int i = 0; i < row_count; ++i) {
    const std::string file_path(rpmdb_table_ptr->get_value(file_path_col_num, i));
    struct stat st;
    if (0 != access(file_path.c_str(), R_OK) || 0 != stat(file_path.c_str(), &st)) {
      continue;
    }
    if (S_ISREG(st.st_mode) && 0 < st.st_size && OK == sha256_file(hash, sizeof(hash), file_path.c_str())) {
      table_ptr->push_back_row({boost::lexical_cast<std::string>(i), rpmdb_table_ptr->get_value(nvra_col_num, i),
                                file_path, uid_as_user(st.st_uid), gid_as_group(st.st_gid), mode_as_string(st.st_mode),
                                boost::lexical_cast<std::string>(st.st_size), hash});
    }
    if (progress_callback && 0 != progress_callback(progress_client_ptr, row_count, i + 1)) {
      break;
    }
  }
  return table_ptr;
}

int compute_file_differences(const table_t *rpmdb_table_ptr, table_t *rpmdb_table_inst_ptr) {
  const int package_row_col_num = rpmdb_table_inst_ptr->find_column_number("PACKAGE_ROW_NUM");

  const int init_user_col_num = rpmdb_table_ptr->find_column_number("FILE_INITIAL_USER");
  const int init_group_col_num = rpmdb_table_ptr->find_column_number("FILE_INITIAL_GROUP");
  const int init_mode_col_num = rpmdb_table_ptr->find_column_number("FILE_INITIAL_MODE");
  const int init_size_col_num = rpmdb_table_ptr->find_column_number("FILE_INITIAL_SIZE");
  const int init_sha26_col_num = rpmdb_table_ptr->find_column_number("FILE_INITIAL_SHA256");

  const int curr_user_col_num = rpmdb_table_inst_ptr->find_column_number("FILE_CURRENT_USER");
  const int curr_group_col_num = rpmdb_table_inst_ptr->find_column_number("FILE_CURRENT_GROUP");
  const int curr_mode_col_num = rpmdb_table_inst_ptr->find_column_number("FILE_CURRENT_MODE");
  const int curr_size_col_num = rpmdb_table_inst_ptr->find_column_number("FILE_CURRENT_SIZE");
  const int curr_sha256_col_num = rpmdb_table_inst_ptr->find_column_number("FILE_CURRENT_SHA256");

  const int row_count = rpmdb_table_inst_ptr->get_row_count();

  vect_of_strings_t user_diff;
  vect_of_strings_t group_diff;
  vect_of_strings_t mode_diff;
  vect_of_strings_t size_diff;
  vect_of_strings_t sha256_diff;
  vect_of_strings_t diff_count;

  user_diff.reserve(row_count);
  group_diff.reserve(row_count);
  mode_diff.reserve(row_count);
  size_diff.reserve(row_count);
  sha256_diff.reserve(row_count);
  diff_count.reserve(row_count);

  if (package_row_col_num < 0 || init_user_col_num < 0 || init_group_col_num < 0 || init_mode_col_num < 0 ||
      init_size_col_num < 0 || init_sha26_col_num < 0 || curr_user_col_num < 0 || curr_group_col_num < 0 ||
      curr_mode_col_num < 0 || curr_size_col_num < 0 || curr_sha256_col_num < 0) {
    abort();
  }
  for (int i = 0; i < row_count; ++i) {
    int diff_cnt = 0;
    int orig_row = boost::lexical_cast<int>(rpmdb_table_inst_ptr->get_value(package_row_col_num, i));

    /* user */
    if (rpmdb_table_ptr->get_value(init_user_col_num, orig_row) !=
        rpmdb_table_inst_ptr->get_value(curr_user_col_num, i)) {
      user_diff.push_back("1");
      ++diff_cnt;
    } else {
      user_diff.push_back("0");
    }
    /* group */
    if (rpmdb_table_ptr->get_value(init_group_col_num, orig_row) !=
        rpmdb_table_inst_ptr->get_value(curr_group_col_num, i)) {
      group_diff.push_back("1");
      ++diff_cnt;
    } else {
      group_diff.push_back("0");
    }
    /* mode */
    if (rpmdb_table_ptr->get_value(init_mode_col_num, orig_row) !=
        rpmdb_table_inst_ptr->get_value(curr_mode_col_num, i)) {
      mode_diff.push_back("1");
      ++diff_cnt;
    } else {
      mode_diff.push_back("0");
    }
    /* size */
    if (rpmdb_table_ptr->get_value(init_size_col_num, orig_row) !=
        rpmdb_table_inst_ptr->get_value(curr_size_col_num, i)) {
      size_diff.push_back("1");
      ++diff_cnt;
    } else {
      size_diff.push_back("0");
    }
    /* sha-256 */
    if (rpmdb_table_ptr->get_value(init_sha26_col_num, orig_row) !=
        rpmdb_table_inst_ptr->get_value(curr_sha256_col_num, i)) {
      sha256_diff.push_back("1");
      ++diff_cnt;
    } else {
      sha256_diff.push_back("0");
    }
    diff_count.push_back(boost::lexical_cast<std::string>(diff_cnt));
  }
  rpmdb_table_inst_ptr->push_back_column("IS_USER_DIFF_FROM_INITIAL", user_diff);
  rpmdb_table_inst_ptr->push_back_column("IS_GROUP_DIFF_FROM_INITIAL", group_diff);
  rpmdb_table_inst_ptr->push_back_column("IS_MODE_DIFF_FROM_INITIAL", mode_diff);
  rpmdb_table_inst_ptr->push_back_column("IS_SIZE_DIFF_FROM_INITIAL", size_diff);
  rpmdb_table_inst_ptr->push_back_column("IS_SHA256_DIFF_FROM_INITIAL", sha256_diff);
  rpmdb_table_inst_ptr->push_back_column("DIFF_FROM_INITIAL_COUNT", diff_count);
  return OK;
}