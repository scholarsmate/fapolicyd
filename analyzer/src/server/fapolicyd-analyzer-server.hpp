#ifndef FAPOLICYD_ANALYZER_SERVER_HPP
#define FAPOLICYD_ANALYZER_SERVER_HPP

#include "./fapolicyd-analyzer-server-adaptor.hpp"
#include <dbus-c++/dbus.h>

class FapolicydAnalyzerServer : public org::freedesktop::DBus::fapolicyd::analyzer_adaptor,
                                public DBus::IntrospectableAdaptor,
                                public DBus::ObjectAdaptor {
public:
  typedef std::vector<::DBus::Struct<std::string, std::vector<std::string>>> table_t;
  FapolicydAnalyzerServer(DBus::Connection &connection);
  table_t RpmDbAsTable();
};

#endif /* FAPOLICYD_ANALYZER_SERVER_HPP */