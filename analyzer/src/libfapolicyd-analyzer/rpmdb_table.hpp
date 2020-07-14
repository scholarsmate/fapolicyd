#ifndef LIBFAPOLICYD_ANALYZER_CXX_RPMDB_TABLE_HPP_
#define LIBFAPOLICYD_ANALYZER_CXX_RPMDB_TABLE_HPP_

#include <libfapolicyd-analyzer/table.hpp>

table_t *rpmdb_read_as_table();
typedef int (*package_progress_callback_t)(void *, int, int);
table_t *rpmdb_hash_files(const table_t *rpmdb_table_ptr, package_progress_callback_t progress_callback,
                          void *progress_client_ptr);
int compute_file_differences(const table_t *rpmdb_table_ptr, table_t *rpmdb_table_inst_ptr);

#endif /* LIBFAPOLICYD_ANALYZER_CXX_RPMDB_TABLE_HPP_ */
