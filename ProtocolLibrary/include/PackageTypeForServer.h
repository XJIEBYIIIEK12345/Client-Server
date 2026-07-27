#ifndef PACKAGETYPEFORSERVER_H
#define PACKAGETYPEFORSERVER_H
#include <QString>
#include <QDataStream>

struct PackageTypeToClient {
    QString valueType;
    quint64 bytes;

    PackageTypeToClient() : valueType("int"), bytes(0) {}
    PackageTypeToClient(QString _valueType, quint64 _bytes) : valueType(_valueType), bytes(_bytes) {}
};

inline QDataStream &operator<<(QDataStream &out, const PackageTypeToClient& packageTypeToClient) {
    out << packageTypeToClient.valueType << packageTypeToClient.bytes;
    return out;
}

#endif // PACKAGETYPEFORSERVER_H
