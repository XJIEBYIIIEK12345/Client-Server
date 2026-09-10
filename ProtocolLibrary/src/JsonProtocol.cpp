#include "JsonProtocol.h"
#include "PackageDataArray.h"
#include "PackageMetaData.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

JsonProtocol::JsonProtocol(log4cplus::Logger logger) { m_logger = logger; }

JsonProtocol::~JsonProtocol()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

QByteArray JsonProtocol::encodeData(Package* pack)
{
  const char endByte = '\n';

  QJsonObject jsonObj = QJsonObject::fromVariantMap(pack->valuesToMap());

  if (jsonObj.contains("data"))
  {
    QJsonArray jsonArr =
        QJsonArray::fromVariantList(jsonObj.value("data").toVariant().toList());

    jsonObj.insert("data", jsonArr);
  }

  QJsonDocument jsonDoc(jsonObj);

  LOG4CPLUS_TRACE(
      m_logger,
      "This send: " << jsonDoc.toJson(QJsonDocument::Indented).toStdString());

  QByteArray data = jsonDoc.toJson(QJsonDocument::Compact);

  data.append(endByte);
  return data;
}

Package* JsonProtocol::decodeData()
{
  const char endByte = '\n';
  int endByteIndex = m_buffer.indexOf(endByte);

  if (endByteIndex < 0)
  {
    return nullptr;
  }
  QJsonParseError* err = nullptr;

  QByteArray tempBuffer;
  tempBuffer = m_buffer.left(endByteIndex);

  QJsonObject jsonObj = QJsonDocument::fromJson(tempBuffer, err).object();

  m_buffer.remove(0, endByteIndex + 1);

  if (err == nullptr)
  {
    switch (MessageType(jsonObj["type"].toInt()))
    {
    case MessageType::MetaDataRequest:
      return new Package(jsonObj.toVariantMap());
      break;
    case MessageType::MetaDataResponse:
      return new PackageMetaData(jsonObj.toVariantMap());
      break;
    case MessageType::SinAnswer:
      return new PackageDataArray(jsonObj.toVariantMap());
      break;
    case MessageType::SinConfirmation:
      return new Package(jsonObj.toVariantMap());
      break;
    case MessageType::Count:
      LOG4CPLUS_WARN(m_logger, "This type of message is unsupported");
      return nullptr;
      break;
    }
  }
  else
  {
    delete err;
    err = nullptr;
    return nullptr;
  }
}
