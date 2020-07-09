#include "table.hpp"
#include "error_codes.h"
#include <algorithm>
#include <boost/numeric/conversion/cast.hpp>

typedef struct column_struct {
  std::string name;
  vect_of_strings_t column;
} column_t;

struct table_t::Impl_t {
  std::vector<column_t> columns;
};

table_t::table_t() : pimpl_(new Impl_t) {}

table_t::~table_t() { delete pimpl_; }

int table_t::initialize(const vect_of_strings_t &column_names) {
  const int column_count = column_names.size();
  if (0 < column_count) {
    pimpl_->columns.reserve(column_count);
    for (int col_index = 0; col_index < column_count; ++col_index) {
      pimpl_->columns.push_back(column_t());
      pimpl_->columns.back().name = column_names[col_index];
    }
    return OK;
  }
  return RANGE_ERROR;
}

int table_t::get_column_count() const {
  try {
    return boost::numeric_cast<int>(pimpl_->columns.size());
  } catch (std::exception &) {
    return RANGE_ERROR;
  }
}

int table_t::get_row_count() const {
  if (0 < get_column_count()) {
    try {
      return boost::numeric_cast<int>(pimpl_->columns[0].column.size());
    } catch (std::exception &) {
      return RANGE_ERROR;
    }
  }
  return NOT_FOUND;
}

std::string table_t::get_column_name(int col_num) const { return pimpl_->columns[col_num].name; }

int table_t::find_column_number(const std::string &column_name) const {
  for (std::vector<column_t>::const_iterator iter(pimpl_->columns.cbegin()); iter != pimpl_->columns.cend(); ++iter) {
    if (column_name == iter->name) {
      try {
        return boost::numeric_cast<int>(std::distance(pimpl_->columns.cbegin(), iter));
      } catch (std::exception &) {
        return RANGE_ERROR;
      }
    }
  }
  return NOT_FOUND;
}

std::string table_t::get_value(int col_num, int row_num) const { return pimpl_->columns[col_num].column[row_num]; }

int table_t::push_back_row(const vect_of_strings_t &row_values) {
  const int column_count = get_column_count();
  if (column_count == row_values.size()) {
    for (int col_index = 0; col_index < column_count; ++col_index) {
      pimpl_->columns[col_index].column.push_back(row_values[col_index]);
    }
    return OK;
  }
  return RANGE_ERROR;
}

int table_t::push_back_column(const std::string &column_name, const vect_of_strings_t &column_values) {
  if (get_row_count() == column_values.size()) {
      pimpl_->columns.push_back(column_t());
      pimpl_->columns.back().name = column_name;
      pimpl_->columns.back().column = column_values;
    return OK;
  }
  return RANGE_ERROR;
}
