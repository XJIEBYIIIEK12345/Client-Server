#ifndef PACKAGETYPE_H
#define PACKAGETYPE_H
#include <QString>
#include <QDataStream>

struct PackageTypeToClient {
    QString valueType;
    quint32 bytes;

    PackageTypeToClient() : valueType("int"), bytes(0) {}
    PackageTypeToClient(QString _valueType, quint32 _bytes) : valueType(_valueType), bytes(_bytes) {}
};

inline QDataStream &operator>>(QDataStream &in, PackageTypeToClient& packageTypeToClient) {
    in >> packageTypeToClient.valueType >> packageTypeToClient.bytes;
    return in;
}

inline QDataStream &operator<<(QDataStream &out, const PackageTypeToClient& packageTypeToClient) {
    out << packageTypeToClient.valueType << packageTypeToClient.bytes;
    return out;
}
#endif // PACKAGETYPE_H
