TEMPLATE = subdirs
 
SUBDIRS = \
        ProtocolLibrary \
        Server \
        Client \

Server.depends = ProtocolLibrary

Client.depends = ProtocolLibrary

CONFIG += link_pkgconfig

PKGCONFIG += log4cplus

