TEMPLATE = subdirs
 
SUBDIRS = \
        ProtocolLibrary \
        Server \
        Client \

Server.depends = ProtocolLibrary

Client.depends = ProtocolLibrary
