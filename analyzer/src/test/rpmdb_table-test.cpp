#include <iostream>
#include <libfapolicyd-analyzer/error_codes.h>
#include <libfapolicyd-analyzer/rpmdb_table.hpp>

#define EMIT_PROGRESS

int emit_progress(void *clientp, int total, int current) {
#ifdef EMIT_PROGRESS
  FILE *fp = (FILE *)clientp;
  if (total && fp) {
    fprintf(fp, "%%%d\r", (int)(100 * current / total));
    fflush(fp);
  }
#endif
  return 0;
}

void print_table(const table_t *table_ptr, std::ostream &os) {
  const int num_cols = table_ptr->get_column_count();
  const int num_rows = table_ptr->get_row_count();
  int i, j;
  os << "ROW_ID";
  for (i = 0; i < num_cols; ++i) {
    os << "\t" << table_ptr->get_column_name(i);
  }
  for (j = 0; j < num_rows; ++j) {
    os << "\n" << j;
    for (i = 0; i < num_cols; ++i) {
      os << "\t" << table_ptr->get_value(i, j);
    }
  }
  os << std::endl;
  os << "Cols: " << num_cols << " Rows: " << num_rows << std::endl;
}

int main(int argc, char **argv) {
  const table_t *rpm_table_ptr = rpmdb_read_as_table(); // takes about 1 second
  print_table(rpm_table_ptr, std::cout);
  table_t *disk_file_table_ptr = rpmdb_hash_files(rpm_table_ptr, emit_progress, stdout); // takes about 30 seconds
  compute_file_differences(rpm_table_ptr, disk_file_table_ptr);                          // near instant
  print_table(disk_file_table_ptr, std::cout);
  delete disk_file_table_ptr;
  delete rpm_table_ptr;
  return 0;
}
