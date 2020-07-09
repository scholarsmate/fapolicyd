#ifndef LIBFAPOLICYD_ANALYZER_CXX_TABLE_HPP_
#define LIBFAPOLICYD_ANALYZER_CXX_TABLE_HPP_

#include <string>
#include <vector>

typedef std::vector<std::string> vect_of_strings_t;

class table_t {
private:
  struct Impl_t;
  Impl_t *pimpl_;

public:
  table_t();
  table_t(const table_t &other);
  ~table_t();
  int initialize(const vect_of_strings_t &column_names);
  int get_column_count() const;
  int get_row_count() const;
  std::string get_column_name(int col_num) const;
  int find_column_number(const std::string &column_name) const;
  std::string get_value(int col_num, int row_num) const;
  int push_back_row(const vect_of_strings_t &row_values);
  int push_back_column(const std::string &column_name, const vect_of_strings_t &column_values);
};

#endif /* LIBFAPOLICYD_ANALYZER_CXX_TABLE_HPP_ */
