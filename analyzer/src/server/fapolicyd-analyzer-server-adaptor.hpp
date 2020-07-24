
/*
 *	This file was automatically generated by dbusxx-xml2cpp; DO NOT EDIT!
 */

#ifndef __dbusxx__fapolicyd_analyzer_server_adaptor_hpp__ADAPTOR_MARSHAL_H
#define __dbusxx__fapolicyd_analyzer_server_adaptor_hpp__ADAPTOR_MARSHAL_H

#include <dbus-c++/dbus.h>
#include <cassert>

namespace org {
namespace freedesktop {
namespace DBus {
namespace fapolicyd {

class analyzer_adaptor
: public ::DBus::InterfaceAdaptor
{
public:

    analyzer_adaptor()
    : ::DBus::InterfaceAdaptor("org.freedesktop.DBus.fapolicyd.analyzer")
    {
        register_method(analyzer_adaptor, RpmDbAsTable, _RpmDbAsTable_stub);
    }

    ::DBus::IntrospectedInterface *introspect() const 
    {
        static ::DBus::IntrospectedArgument RpmDbAsTable_args[] = 
        {
            { "rpmTable", "a(sas)", false },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedMethod analyzer_adaptor_methods[] = 
        {
            { "RpmDbAsTable", RpmDbAsTable_args },
            { 0, 0 }
        };
        static ::DBus::IntrospectedMethod analyzer_adaptor_signals[] = 
        {
            { 0, 0 }
        };
        static ::DBus::IntrospectedProperty analyzer_adaptor_properties[] = 
        {
            { 0, 0, 0, 0 }
        };
        static ::DBus::IntrospectedInterface analyzer_adaptor_interface = 
        {
            "org.freedesktop.DBus.fapolicyd.analyzer",
            analyzer_adaptor_methods,
            analyzer_adaptor_signals,
            analyzer_adaptor_properties
        };
        return &analyzer_adaptor_interface;
    }

public:

    /* properties exposed by this interface, use
     * property() and property(value) to get and set a particular property
     */

public:

    /* methods exported by this interface,
     * you will have to implement them in your ObjectAdaptor
     */
    virtual std::vector< ::DBus::Struct< std::string, std::vector< std::string > > > RpmDbAsTable() = 0;

public:

    /* signal emitters for this interface
     */

private:

    /* unmarshalers (to unpack the DBus message before calling the actual interface method)
     */
    ::DBus::Message _RpmDbAsTable_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        std::vector< ::DBus::Struct< std::string, std::vector< std::string > > > argout1 = RpmDbAsTable();
        ::DBus::ReturnMessage reply(call);
        ::DBus::MessageIter wi = reply.writer();
        wi << argout1;
        return reply;
    }
};

} } } } 
#endif //__dbusxx__fapolicyd_analyzer_server_adaptor_hpp__ADAPTOR_MARSHAL_H