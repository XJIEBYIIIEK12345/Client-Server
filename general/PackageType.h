#ifndef PACKAGETYPE_H
#define PACKAGETYPE_H
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

inline QDataStream &operator>>(QDataStream &in, PackageTypeToClient& packageTypeToClient) {
    in >> packageTypeToClient.valueType >> packageTypeToClient.bytes;
    return in;
}

// struct PackageTypeToServer {
//     QByteArray block;
//     QString typeSize;

//     PackageTypeToServer() : typeSize("") {}
//     PackageTypeToServer(QByteArray _block, QString _typeSize) : block(_block), typeSize(_typeSize) {}
// };

// inline QDataStream &operator<<(QDataStream &out, const PackageTypeToServer& packageTypeToServer) {
//     out << packageTypeToServer.block << packageTypeToServer.typeSize;
//     return out;
// }

// inline QDataStream &operator>>(QDataStream &in, PackageTypeToServer& packageTypeToServer) {
//     in >> packageTypeToServer.block >> packageTypeToServer.typeSize;
//     return in;
// }
#endif // PACKAGETYPE_H
