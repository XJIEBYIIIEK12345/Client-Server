#include "JsonProtocol.h"
#include "PackageForDataToGenerate.h"
#include "PackageForGeneratedData.h"
#include "PackageForSignal.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

JsonProtocol::JsonProtocol() {}

JsonProtocol::~JsonProtocol() {}

QByteArray JsonProtocol::encodeData(Package* pack)
{
  const char frontByte = 0x02;
  const char endByte = 0x03;

  QJsonObject jsonObj = QJsonObject::fromVariantMap(pack->valuesToMap());

  QJsonDocument jsonDoc(jsonObj);
  QByteArray data = jsonDoc.toJson();

  data.prepend(frontByte);
  data.append(endByte);

  return data;
}

Package* JsonProtocol::decodeData()
{
  const char frontByte = 0x02;
  const char endByte = 0x03;

  int frontByteIndex = m_buffer.indexOf(frontByte);
  int endByteIndex = m_buffer.indexOf(endByte);
  int count = endByteIndex - frontByteIndex;

  if (frontByteIndex < 0 || endByteIndex < 0)
  {
    return nullptr;
  }
  QJsonParseError* err = nullptr;

  QByteArray tempBuffer;
  tempBuffer.insert(frontByteIndex, m_buffer, count);
  tempBuffer.remove(0, 1);

  QJsonObject jsonObj = QJsonDocument::fromJson(tempBuffer, err).object();

  m_buffer.remove(frontByteIndex, count + 1);

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
      qDebug() << "This type of message is unsupported";
      return nullptr;
      break;
    }
  }
  else
    return nullptr;
}
