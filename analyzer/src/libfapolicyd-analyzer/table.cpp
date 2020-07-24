#include "table.hpp"
#include "error_codes.h"
#include <boost/numeric/conversion/cast.hpp>
#include <utility>

typedef std::pair<std::string, vect_of_strings_t> column_t;
typedef std::vector<column_t> columns_t;
struct table_t::Impl_t {
  columns_t columns;
};

table_t::table_t() : pimpl_(new Impl_t) {}

table_t::~table_t() { delete pimpl_; }

int table_t::initialize(const vect_of_strings_t &column_names) {
  const int column_count = column_names.size();
  if (0 < column_count) {
    pimpl_->columns.reserve(column_count);
    for (int col_index = 0; col_index < column_count; ++col_index) {
      pimpl_->columns.push_back(std::make_pair(column_names[col_index], vect_of_strings_t()));
    }
    return OK;
  }
  return RANGE_ERROR;
}

int table_t::get_row_count() const {
  if (0 < get_column_count()) {
    try {
      return boost::numeric_cast<int>(pimpl_->columns[0].second.size());
    } catch (std::exception &) {
      return RANGE_ERROR;
    }
  }
  return NOT_FOUND;
}

int table_t::get_column_count() const {
  try {
    return boost::numeric_cast<int>(pimpl_->columns.size());
  } catch (std::exception &) {
    return RANGE_ERROR;
  }
}

std::string table_t::get_column_name(int col_num) const { return pimpl_->columns[col_num].first; }

vect_of_strings_t table_t::get_column(int col_num) const { return pimpl_->columns[col_num].second; }

int table_t::find_column_number(const std::string &column_name) const {
  for (columns_t::const_iterator iter(pimpl_->columns.cbegin()); iter != pimpl_->columns.cend(); ++iter) {
    if (column_name == iter->first) {
      try {
        return boost::numeric_cast<int>(std::distance(pimpl_->columns.cbegin(), iter));
      } catch (std::exception &) {
        return RANGE_ERROR;
      }
    }
  }
  return NOT_FOUND;
}

std::string table_t::get_value(int col_num, int row_num) const { return pimpl_->columns[col_num].second[row_num]; }

int table_t::push_back_row(const vect_of_strings_t &row_values) {
  const int column_count = get_column_count();
  if (column_count != row_values.size()) {
    return RANGE_ERROR;
  }
  for (int col_index = 0; col_index < column_count; ++col_index) {
    pimpl_->columns[col_index].second.push_back(row_values[col_index]);
  }
  return OK;
}

int table_t::push_back_column(const std::string &column_name, const vect_of_strings_t &column_values) {
  if (get_row_count() != column_values.size()) {
    return RANGE_ERROR;
  }
  pimpl_->columns.push_back(std::make_pair(column_name, column_values));
  return OK;
}
