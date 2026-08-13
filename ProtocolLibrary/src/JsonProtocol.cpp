#include "JsonProtocol.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

JsonProtocol::JsonProtocol() {}

JsonProtocol::~JsonProtocol() {}

QByteArray JsonProtocol::encodeData(Package* pack)
{
  QJsonObject jsonObj = {{"count", pack->m_count},
                         {"type", pack->m_type},
                         {"data", QString(pack->m_data.toBase64())}};

  QJsonDocument jsonDoc(jsonObj);
  QByteArray data = jsonDoc.toJson();

  return data;
}

Package* JsonProtocol::decodeData()
{
  QJsonParseError* err = nullptr;

  QJsonObject jsonObj = QJsonDocument::fromJson(m_buffer, err).object();

  if (err == nullptr)
  {

    Package* pack =
        new Package(jsonObj["count"].toInt(), jsonObj["type"].toString(),
                    QByteArray::fromBase64(jsonObj["data"].toString().toUtf8()));

    return pack;
  }
  else
    return nullptr;
}
