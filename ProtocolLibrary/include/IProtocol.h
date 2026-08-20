#ifndef IPROTOCOL_H
#define IPROTOCOL_H

#include "Package.h"
#include <QByteArray>
#include <QDebug>
#include <QString>

enum ProtocolDataType
{
  JsonType = 0,
  BinType,
  XmlType
};

class IProtocol
{
public:
  virtual ~IProtocol() = default;

  static IProtocol* makeProtocol(ProtocolDataType type);
  static IProtocol* makeProtocol(QString type);
  static QString toString(ProtocolDataType type);
  virtual QByteArray encodeData(Package* pack) = 0;
  virtual Package* decodeData() = 0;
  QByteArray processMessage(QByteArray message);

  QByteArray m_buffer;
};

#endif // IPROTOCOL_H
