#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>

#include "fapolicyd-analyzer-server.hpp"
#include <libfapolicyd-analyzer/error_codes.h>
#include <libfapolicyd-analyzer/rpmdb_table.hpp>

static const char *FAPOLICY_ANALYZER_SERVER_NAME = "org.freedesktop.DBus.fapolicyd.analyzer";
static const char *FAPOLICY_ANALYZER_SERVER_PATH = "/org/freedesktop/DBus/fapolicyd/analyzer";

static inline FapolicydAnalyzerServer::table_t convert_table(const ::table_t *table_ptr) {
  const int num_cols (table_ptr->get_column_count());
  FapolicydAnalyzerServer::table_t result;
  result.reserve(num_cols);
  for (int col_num (0); col_num < num_cols; ++col_num) {
    result.push_back({table_ptr->get_column_name(col_num), table_ptr->get_column(col_num)});
  }
  return result;
}

FapolicydAnalyzerServer::FapolicydAnalyzerServer(DBus::Connection &connection)
    : DBus::ObjectAdaptor(connection, FAPOLICY_ANALYZER_SERVER_PATH) {}

FapolicydAnalyzerServer::table_t FapolicydAnalyzerServer::RpmDbAsTable() {
  return convert_table(rpmdb_read_as_table());
}

static DBus::BusDispatcher dispatcher;

static void signal_handler(int sig) { dispatcher.leave(); }

int main() {
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  DBus::default_dispatcher = &dispatcher;

  // TODO: Make this use SystemBus
  DBus::Connection conn (DBus::Connection::SessionBus());
  conn.request_name(FAPOLICY_ANALYZER_SERVER_NAME);
  FapolicydAnalyzerServer server(conn);

  dispatcher.enter();

  return EXIT_SUCCESS;
}
