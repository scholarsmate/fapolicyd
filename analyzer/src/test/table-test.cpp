#include <iostream>
#include <boost/lexical_cast.hpp>
#include <libfapolicyd-analyzer/table.hpp>
#include <libfapolicyd-analyzer/error_codes.h>

void print_table(const table_t &table, std::ostream &os) {
    const int num_cols = table.get_column_count();
    const int num_rows = table.get_row_count();
    int i, j;
    os << "ROW_ID";
    for(i = 0; i < num_cols; ++i) {
        os << "\t" << table.get_column_name(i);
    }
    for (j = 0; j < num_rows; ++j) {
        os << "\n" << j + 1;
        for(i = 0; i < num_cols; ++i) {
            os << "\t" << table.get_value(i, j);
        }
    }
    os << std::endl;
    os << "Cols: " << num_cols << " Rows: " << num_rows << std::endl;
}

int main() {
    const int col_count = 10;
    const int row_count = 1000;
    int i, j;
    table_t table;

    vect_of_strings_t colnames;
    for(i = 0; i < col_count; ++i) {
        std::string cname("column_");
        cname += boost::lexical_cast<std::string>(i);
        colnames.push_back(cname);
    }
    if (OK != table.initialize(colnames)) abort ();
    for(j = 0; j < row_count; ++j) {
        vect_of_strings_t rowdata;
        for(i = 0; i < col_count; ++i) {
            std::string value("row_data ");
            value += boost::lexical_cast<std::string>((j * col_count + i) % 10001);
            rowdata.push_back(value);
        }
        std::cout << ".";
        if (OK != table.push_back_row(rowdata)) abort();
    }
    std::cout << std::endl;
    print_table(table, std::cout);
    return 0;
}