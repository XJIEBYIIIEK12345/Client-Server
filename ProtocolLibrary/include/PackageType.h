#ifndef PACKAGETYPE_H
#define PACKAGETYPE_H
#include <QString>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

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

QJsonObject createJsonObject(qint32 count, const QString& type, const QByteArray& data) {

    QJsonObject message = {
        {"count", count},
        {"type", type},
        {"data", QString(data.toBase64())}
    };

    return message;
}

QByteArray jsonObjToByteArray(QJsonObject jsonObj) {

    QJsonDocument jsonDoc(jsonObj);
    QByteArray data = jsonDoc.toJson();

    return data;
}

QJsonObject byteArrayToJsonObj(const QByteArray& data) {

    QJsonObject message = QJsonDocument::fromJson(data).object();
    return message;
}

#endif // PACKAGETYPE_H
