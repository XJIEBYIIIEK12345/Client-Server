#include "XmlProtocol.h"
#include "Package.h"
#include "PackageMetaData.h"
#include "PackageDataArray.h"
#include <QDomDocument>
#include <QDomElement>

XmlProtocol::XmlProtocol(log4cplus::Logger logger) { m_logger = logger; }

XmlProtocol::~XmlProtocol()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

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
  }
  break;
  case MessageType::MetaDataResponse:
  {
    PackageMetaData* package = dynamic_cast<PackageMetaData*>(pack);
    QDomElement valueType = doc.createElement("valueType");
    valueType.appendChild(doc.createTextNode(package->m_valueType));
    root.appendChild(valueType);
    QDomElement bytes = doc.createElement("bytes");
    bytes.appendChild(doc.createTextNode(QString::number(package->m_bytes)));
    root.appendChild(bytes);

    if (package != nullptr)
    {
      package = nullptr;
    }
  }
  break;
  case MessageType::SinAnswer:
  {
    PackageDataArray* package = dynamic_cast<PackageDataArray*>(pack);
    QDomElement data = doc.createElement("data");
    for (int i = 0; i < package->m_data.size(); ++i)
    {
      QDomElement dataElement = doc.createElement("value");
      dataElement.appendChild(doc.createTextNode(package->m_data[i].toString()));
      data.appendChild(dataElement);
    }
    root.appendChild(data);

    if (package != nullptr)
    {
      package = nullptr;
    }
  }
  break;
  case MessageType::SinConfirmation:
  {
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
      return new Package(id, messageType);
      break;
    case MessageType::MetaDataResponse:
      return new PackageMetaData(id, root.firstChildElement("valueType").text(),
                                 root.firstChildElement("bytes").text().toInt());
      break;
    case MessageType::SinAnswer:
    {
      QVariantList variantList;
      QDomNodeList xmlList = doc.elementsByTagName("value");

      variantList.reserve(xmlList.size());

      for (int i = 0; i < xmlList.size(); ++i)
      {
        QDomNode node = xmlList.at(i);
        if (node.isElement())
        {
          QString element = node.toElement().text();

          bool isInt = false;
          bool isDouble = false;

          qint64 intVal = element.toLongLong(&isInt);

          if (isInt)
          {
            variantList.append(QVariant::fromValue(intVal));
          }
          else
          {
            double doubleVal = element.toDouble(&isDouble);
            if (isDouble)
            {
              variantList.append(QVariant::fromValue(doubleVal));
            }
            else
            {
              variantList.append(QVariant::fromValue(element));
            }
          }
        }
      }
      return new PackageDataArray(id, variantList);
    }
    break;
    case MessageType::SinConfirmation:
      return new Package(id, messageType);
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
