#include "system_trust.hpp"
#include "detail/macros.h"
#include "error_codes.h"
#include "sha256.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include <sys/stat.h>
#include <vector>

typedef std::vector<std::string> vect_of_strings_t;

table_t *package_get_rpm_info() {
  static const char *command = "/usr/bin/rpm -qa --queryformat \""
                               "%{NVRA}\t%{NAME}\t%{VERSION}\t%{RELEASE}\t%{ARCH}\t"
                               "%{SIZE}\t%{SHA256HEADER}\t%{BUILDTIME}\t%{INSTALLTIME}\t%{GROUP}\t"
                               "%{URL}\t%{RPMVERSION}\t%{SUMMARY}\t"
                               "%|DSAHEADER?{%{DSAHEADER:pgpsig}}:{%|RSAHEADER?{%{RSAHEADER:pgpsig}}:{%|"
                               "SIGGPG?{%{SIGGPG:pgpsig}}:{%|SIGPGP?{%{SIGPGP:pgpsig}}:{(none)}|}|}|}|"
                               "\n\"";
  static const vect_of_strings_t column_names = {"NVRA", "NAME",         "VERSION",   "RELEASE",     "ARCH",
                                                 "SIZE", "SHA256HEADER", "BUILDTIME", "INSTALLTIME", "GROUP",
                                                 "URL",  "RPMVERSION",   "SUMMARY",   "SIGNATURE"};
  static const size_t column_count = column_names.size();
  const char *crecord[column_count];
  FILE *fp = NULL;

  CHECK_PTR(fp = popen(command, "r"));
  if (fp) {
    table_t *table_ptr(new table_t());
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *probe = NULL;
    char *cptr = NULL;
    size_t col_num = 0;

    table_ptr->initialize(column_names);
    while ((read = getline(&line, &len, fp)) > 0) {
      if (line[read - 1] == '\n') {
        line[read - 1] = '\0';
      }
      cptr = line;
      for (col_num = 0; col_num < column_count - 1; ++col_num) {
        probe = strchr(cptr, '\t');
        *probe = '\0';
        crecord[col_num] = cptr;
        cptr = probe + 1;
      }
      crecord[col_num] = cptr;
      const vect_of_strings_t record(crecord, crecord + (sizeof(crecord) / sizeof(*crecord)));
      table_ptr->push_back_row(record);
    }
    free(line);
    pclose(fp);
    return table_ptr;
  }
  return NULL;
}

table_t *package_get_rpm_files_info(const table_t *rpm_table_ptr, package_progress_callback_t progress_callback,
                                    void *progress_client_ptr) {
  int nvra_col_num = 0;
  const int row_count = rpm_table_ptr->get_row_count();
  const std::string command_pfx("/usr/bin/rpm -ql --dump ");
  table_t *table_ptr = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *probe = NULL;
  char *cptr = NULL;
  int col_num = 0;

  static const vect_of_strings_t column_names({"NVRA", "Path", "Size", "Field_3", "SHA256", "Permissions", "User",
                                               "Group", "Field_8", "Field_9", "Field_10", "Field_11"});
  static const size_t column_count = column_names.size();
  const char *crecord[column_count];

  CHECK_PTR(table_ptr = new table_t());
  table_ptr->initialize(column_names);

  if (0 <= (nvra_col_num = table_ptr->find_column_number("NVRA"))) {
    abort();
  };

  for (int i = 0; i < row_count; ++i) {
    const std::string nvra(table_ptr->get_value(nvra_col_num, i));
    const std::string command(command_pfx + nvra);
    FILE *fp = NULL;

    CHECK_PTR(fp = popen(command.c_str(), "r"));
    while ((read = getline(&line, &len, fp)) > 0) {
      if (line[0] == '(') { /* (contains no file) */
        continue;
      }
      if (line[read - 1] == '\n') {
        line[read - 1] = '\0';
      }
      crecord[0] = nvra.c_str();
      cptr = line;
      for (col_num = 1; col_num < column_count - 1; ++col_num) {
        probe = strchr(cptr, ' ');
        *probe = '\0';
        crecord[col_num] = cptr;
        cptr = probe + 1;
      }
      crecord[col_num] = cptr;
      const vect_of_strings_t record(crecord, crecord + (sizeof(crecord) / sizeof(*crecord)));
      table_ptr->push_back_row(record);
    }
    pclose(fp);
    if (progress_callback && 0 != progress_callback(progress_client_ptr, row_count, i + 1)) {
      break;
    }
  }
  free(line);
  return table_ptr;
}

table_t *package_hash_files(const table_t *rpm_files_info_table_ptr, package_progress_callback_t progress_callback,
                            void *progress_client_ptr) {
  const int row_count = rpm_files_info_table_ptr->get_row_count();
  const int file_path_col_num = rpm_files_info_table_ptr->find_column_number("Path");
  const int nvra_col_num = rpm_files_info_table_ptr->find_column_number("NVRA");
  char hash[64];
  table_t *table_ptr(new table_t());

  if (row_count < 0 || file_path_col_num < 0 || nvra_col_num < 0) {
    abort();
  }
  table_ptr->initialize({"NVRA", "Path", "Size", "SHA256"});
  for (int i = 0; i < row_count; ++i) {
    const std::string file_path = rpm_files_info_table_ptr->get_value(file_path_col_num, i);
    struct stat st;
    if (0 != access(file_path.c_str(), R_OK) || 0 != stat(file_path.c_str(), &st)) {
      continue;
    }
    if (S_ISREG(st.st_mode) && 0 < st.st_size && OK == sha256_file(hash, sizeof(hash), file_path.c_str())) {
      table_ptr->push_back_row({rpm_files_info_table_ptr->get_value(nvra_col_num, i), file_path,
                                boost::lexical_cast<std::string>(st.st_size), hash});
    }
    if (progress_callback && 0 != progress_callback(progress_client_ptr, row_count, i + 1)) {
      break;
    }
  }
  return table_ptr;
}
