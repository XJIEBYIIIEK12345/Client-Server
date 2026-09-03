#include "JsonProtocol.h"
#include "PackageForDataToGenerate.h"
#include "PackageForGeneratedData.h"
#include "PackageForSignal.h"
#include <QJsonDocument>
#include <QJsonObject>

JsonProtocol::JsonProtocol(log4cplus::Logger logger) { m_logger = logger; }

JsonProtocol::~JsonProtocol() {}

QByteArray JsonProtocol::encodeData(Package* pack)
{
  const char endByte = '\n';

  QJsonObject jsonObj = QJsonObject::fromVariantMap(pack->valuesToMap());

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
      return new PackageForSignal(jsonObj.toVariantMap());
      break;
    case MessageType::MetaDataResponse:
      return new PackageForDataToGenerate(jsonObj.toVariantMap());
      break;
    case MessageType::SinAnswer:
      return new PackageForGeneratedData(jsonObj.toVariantMap());
      break;
    case MessageType::SinConfirmation:
      return new PackageForSignal(jsonObj.toVariantMap());
      break;
    case MessageType::Count:
      LOG4CPLUS_WARN(m_logger, "This type of message is unsupported");
      return nullptr;
      break;
    }
  }
  else
    return nullptr;
}
