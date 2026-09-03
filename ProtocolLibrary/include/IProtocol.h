#ifndef IPROTOCOL_H
#define IPROTOCOL_H

#include "Package.h"
#include <QByteArray>
#include <QString>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

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

  static IProtocol* makeProtocol(ProtocolDataType type, log4cplus::Logger logger);
  static IProtocol* makeProtocol(QString type, log4cplus::Logger logger);
  static QString toString(ProtocolDataType type);
  virtual QByteArray encodeData(Package* pack) = 0;
  virtual Package* decodeData() = 0;

  QByteArray m_buffer;
  log4cplus::Logger m_logger;
};

#endif // IPROTOCOL_H
