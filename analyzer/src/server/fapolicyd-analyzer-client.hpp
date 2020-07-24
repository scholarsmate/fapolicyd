#ifndef FAPOLICYD_ANALYZER_CLIENT_HPP
#define FAPOLICYD_ANALYZER_CLIENT_HPP

#include "./fapolicyd-analyzer-client-proxy.hpp"
#include <dbus-c++/dbus.h>

class FapolicydAnalyzerClient : public org::freedesktop::DBus::fapolicyd::analyzer_proxy,
                                public DBus::IntrospectableProxy,
                                public DBus::ObjectProxy {
public:
  typedef std::vector<::DBus::Struct<std::string, std::vector<std::string>>> table_t;
  FapolicydAnalyzerClient(DBus::Connection &connection, const char *path, const char *name);
};

#endif /* FAPOLICYD_ANALYZER_CLIENT_HPP */