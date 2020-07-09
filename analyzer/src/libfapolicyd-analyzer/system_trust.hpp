#ifndef LIBFAPOLICYD_ANALYZER_CXX_SYSTEM_TRUST_HPP_
#define LIBFAPOLICYD_ANALYZER_CXX_SYSTEM_TRUST_HPP_

#include <libfapolicyd-analyzer/table.hpp>

typedef int (*package_progress_callback_t)(void *, int, int);

table_t *package_get_rpm_info();
table_t *package_get_rpm_files_info(const table_t *rpm_table_ptr, package_progress_callback_t progress_callback,
                                    void *progress_client_ptr);
table_t *package_hash_files(const table_t *rpm_files_info_table_ptr, package_progress_callback_t progress_callback,
                            void *progress_client_ptr);

#endif /*  LIBFAPOLICYD_ANALYZER_CXX_SYSTEM_TRUST_HPP_ */
