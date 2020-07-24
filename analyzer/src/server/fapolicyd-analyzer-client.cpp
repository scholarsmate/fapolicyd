#include "fapolicyd-analyzer-client.hpp"
#include <iostream>
#include <signal.h>

FapolicydAnalyzerClient::FapolicydAnalyzerClient(DBus::Connection &connection, const char *path, const char *name)
    : DBus::ObjectProxy(connection, path, name) {}

static const char *FAPOLICY_ANALYZER_SERVER_NAME = "org.freedesktop.DBus.fapolicyd.analyzer";
static const char *FAPOLICY_ANALYZER_SERVER_PATH = "/org/freedesktop/DBus/fapolicyd/analyzer";

void emit_table(const FapolicydAnalyzerClient::table_t &table) {
  const int num_cols(table.size());
  const int num_rows(table[0]._2.size());
  std::cout << "ROW_NUM";
  for (int col(0); col < num_cols; ++col) {
    std::cout << '\t' << table[col]._1;
  }
  std::cout << std::endl;
  for (int row(0); row < num_rows; ++row) {
    std::cout << row;
    for (int col(0); col < num_cols; ++col) {
      std::cout << '\t' << table[col]._2[row];
    }
    std::cout << std::endl;
  }
}

int main() {
  DBus::BusDispatcher dispatcher;
  DBus::default_dispatcher = &dispatcher;

  // TODO: Make this use SystemBus
  DBus::Connection conn(DBus::Connection::SessionBus());

  FapolicydAnalyzerClient fap(conn, FAPOLICY_ANALYZER_SERVER_PATH, FAPOLICY_ANALYZER_SERVER_NAME);

  emit_table(fap.RpmDbAsTable());

  conn.disconnect();
  return 0;
}