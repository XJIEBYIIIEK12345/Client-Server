#ifndef IPROTOCOL_H
#define IPROTOCOL_H

#include "Package.h"
#include <QByteArray>
#include <QDebug>
#include <QString>

enum class ProtocolDataType
{
  JsonType = 0,
  BinType,
  XmlType,
  Count
};

class IProtocol
{
public:
  virtual ~IProtocol() = default;

  static IProtocol* makeProtocol(ProtocolDataType type);
  static IProtocol* makeProtocol(QString type);
  virtual QByteArray encodeData(Package* pack) = 0;
  virtual Package* decodeData() = 0;

  virtual QByteArray generateMessage(qint32 count, MessageType type,
                                     QByteArray data) = 0;

  QByteArray m_buffer;
};

#endif // IPROTOCOL_H
