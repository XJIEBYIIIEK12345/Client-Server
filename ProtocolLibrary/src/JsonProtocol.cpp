#include "JsonProtocol.h"
#include "IProtocol.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

JsonProtocol::JsonProtocol() {}

JsonProtocol::~JsonProtocol() {}

QByteArray JsonProtocol::encodeData() {

    QJsonObject jsonObj = {
        {"count", m_package.count},
        {"type", m_package.type},
        //{"data", QString(m_package.data)}
        {"data", QString(m_package.data.toBase64())}
    };

    QJsonDocument jsonDoc(jsonObj);
    QByteArray data = jsonDoc.toJson();

    return data;
}

bool JsonProtocol::decodeData() {

    QJsonParseError* err = nullptr;

    QJsonObject jsonObj = QJsonDocument::fromJson(m_buffer, err).object();

    if (err == nullptr) {
        m_package.count = jsonObj["count"].toInt();
        m_package.type = jsonObj["type"].toString();
        //m_package.data = jsonObj["data"].toString().toUtf8();
        m_package.data = QByteArray::fromBase64(jsonObj["data"].toString().toUtf8());

        return true;
    }
    else return false;
}
