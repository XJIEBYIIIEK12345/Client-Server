#include "XmlProtocol.h"
#include "Package.h"
#include "PackageForDataToGenerate.h"
#include "PackageForGeneratedData.h"
#include "PackageForSignal.h"
#include <QDomDocument>
#include <QDomElement>

XmlProtocol::XmlProtocol(log4cplus::Logger logger) { m_logger = logger; }

XmlProtocol::~XmlProtocol() {}

QByteArray XmlProtocol::encodeData(Package* pack)
{
  QDomDocument doc;

  QDomElement root = doc.createElement("package");
  doc.appendChild(root);

  QDomElement id = doc.createElement("id");
  id.appendChild(doc.createTextNode(QString::number(pack->m_id)));
  root.appendChild(id);

  QDomElement type = doc.createElement("type");
  type.appendChild(doc.createTextNode(QString::number(pack->m_type)));
  root.appendChild(type);

  switch (pack->m_type)
  {
  case MessageType::MetaDataRequest:
  {
    PackageForSignal* package = dynamic_cast<PackageForSignal*>(pack);
    QDomElement flag = doc.createElement("flag");
    flag.appendChild(doc.createTextNode(QString::number(package->m_flag)));
    root.appendChild(flag);
  }
  break;
  case MessageType::MetaDataResponse:
  {
    PackageForDataToGenerate* package =
        dynamic_cast<PackageForDataToGenerate*>(pack);
    QDomElement valueType = doc.createElement("valueType");
    valueType.appendChild(doc.createTextNode(package->m_valueType));
    root.appendChild(valueType);
    QDomElement bytes = doc.createElement("bytes");
    bytes.appendChild(doc.createTextNode(QString::number(package->m_bytes)));
    root.appendChild(bytes);
  }
  break;
  case MessageType::SinAnswer:
  {
    PackageForGeneratedData* package = dynamic_cast<PackageForGeneratedData*>(pack);
    QDomElement data = doc.createElement("data");
    data.appendChild(doc.createTextNode(QString(package->m_data.toBase64())));
    root.appendChild(data);
  }
  break;
  case MessageType::SinConfirmation:
  {
    PackageForSignal* package = dynamic_cast<PackageForSignal*>(pack);
    QDomElement flag = doc.createElement("flag");
    flag.appendChild(doc.createTextNode(QString::number(package->m_flag)));
    root.appendChild(flag);
  }
  break;
  default:
    LOG4CPLUS_WARN(m_logger, "This type of message is unsupported");
    return nullptr;
  }

  LOG4CPLUS_TRACE(m_logger, "This send: " << doc.toString().toStdString());

  QByteArray data = doc.toByteArray();
  return data;
}

Package* XmlProtocol::decodeData()
{
  const char endTag[] = "/package";
  int endTagStartIndex = m_buffer.indexOf(endTag);

  if (endTagStartIndex < 0)
  {
    return nullptr;
  }

  int endTagEndIndex = endTagStartIndex + sizeof(endTag) + 1;

  QByteArray tempBuffer;
  tempBuffer = m_buffer.left(endTagEndIndex);
  m_buffer.remove(0, endTagEndIndex);

  QDomDocument doc;
  QString errorMsg;
  int errorLine, errorColumn;

  if (doc.setContent(tempBuffer, &errorMsg, &errorLine, &errorColumn))
  {
    QDomElement root = doc.documentElement();
    if (root.tagName() != "package")
    {
      return nullptr;
    }

    MessageType messageType =
        MessageType(root.firstChildElement("type").text().toInt());
    qint32 id = root.firstChildElement("type").text().toInt();

    switch (messageType)
    {
    case MessageType::MetaDataRequest:
      return new PackageForSignal(id, messageType,
                                  root.firstChildElement("flag").text().toInt());
      break;
    case MessageType::MetaDataResponse:
      return new PackageForDataToGenerate(
          id, messageType, root.firstChildElement("valueType").text(),
          root.firstChildElement("bytes").text().toInt());
      break;
    case MessageType::SinAnswer:
      return new PackageForGeneratedData(
          id, messageType,
          QByteArray::fromBase64(root.firstChildElement("data").text().toUtf8()));
      break;
    case MessageType::SinConfirmation:
      return new PackageForSignal(id, messageType,
                                  root.firstChildElement("flag").text().toInt());
      break;
    case MessageType::Count:
      LOG4CPLUS_WARN(m_logger, "This type of message is unsupported");
      return nullptr;
      break;
    }
  }
  else
  {
    LOG4CPLUS_ERROR(m_logger, "Message: " << errorMsg.toStdString()
                                          << " in line: " << errorLine
                                          << ", column: " << errorColumn);
    return nullptr;
  }
}
