/*****************************************************************************
 * TESTING
 ****************************************************************************/

#include <libfapolicyd-analyzer/system_trust.hpp>
#include <stdio.h>
#include <stdlib.h>

int emit_progress(void *clientp, int total, int current) {
  FILE *fp = (FILE *)clientp;
  if (total && fp) {
    fprintf(fp, "%%%d\r", (int)(100 * current / total));
    fflush(fp);
  }
  return 0;
}

void print_table(const table_t *table_ptr, FILE *fp) {
  const int column_count = table_ptr->get_column_count();
  const int row_count = table_ptr->get_row_count();

  fprintf(fp, "ROW_ID");
  for (int i = 0; i < column_count; ++i) {
    fprintf(fp, "\t%s", table_ptr->get_column_name(i).c_str());
  }
  for (int i = 0; i < row_count; ++i) {
    fprintf(fp, "\n%u", i + 1);
    for (int j = 0; j < column_count; ++j) {
      fprintf(fp, "\t%s", table_ptr->get_value(j, i).c_str());
    }
  }
  fprintf(fp, "\n");
}

int main(int argc, char **argv) {

  table_t *rpm_table_ptr(package_get_rpm_info());
  print_table(rpm_table_ptr, stdout);
  printf("\n--------\n");

  table_t *rpm_file_table_ptr(package_get_rpm_files_info(rpm_table_ptr, emit_progress, stdout));
  print_table(rpm_file_table_ptr, stdout);
  printf("\n--------\n");

  table_t *disk_file_table_ptr(package_hash_files(rpm_file_table_ptr, emit_progress, stdout));
  print_table(disk_file_table_ptr, stdout);
  printf("\n--------\n");

  delete disk_file_table_ptr;
  delete rpm_file_table_ptr;
  delete rpm_table_ptr;

  return EXIT_SUCCESS;
}
