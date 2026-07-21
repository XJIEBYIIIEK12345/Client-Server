#ifndef PACKAGETYPE_H
#define PACKAGETYPE_H
#include <QString>
#include <QDataStream>
struct PackageType {
    QString valueType;
    quint64 bytes;

    PackageType() : valueType("int"), bytes(0) {}
    PackageType(QString _valueType, quint64 _bytes) : valueType(_valueType), bytes(_bytes) {}
};

inline QDataStream &operator<<(QDataStream &out, const PackageType& packageType) {
    out << packageType.valueType << packageType.bytes;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, PackageType& packageType) {
    in >> packageType.valueType >> packageType.bytes;
    return in;
}
#endif // PACKAGETYPE_H
