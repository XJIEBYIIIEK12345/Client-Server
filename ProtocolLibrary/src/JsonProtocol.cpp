#include "JsonProtocol.h"
#include <QJsonDocument>

JsonProtocol::JsonProtocol() {

    m_jsonObj = {
        {"count", 0},
        {"type", ""},
        {"data", ""}
    };
}

JsonProtocol::JsonProtocol(qint32 count, const QString& type, const QByteArray& data) {

    m_jsonObj = {
        {"count", count},
        {"type", type},
        {"data", QString(data.toBase64())}
    };
}

JsonProtocol::~JsonProtocol() {}

QByteArray JsonProtocol::encodeData() {

    QJsonDocument jsonDoc(m_jsonObj);
    QByteArray data = jsonDoc.toJson();

    return data;
}

bool JsonProtocol::decodeData() {

    QJsonParseError* err = nullptr;

    m_jsonObj = QJsonDocument::fromJson(m_buffer, err).object();

    if (err == nullptr)
        return true;
    else return false;
}
