#ifndef MESSAGEPROCESSORFORSERVER_H
#define MESSAGEPROCESSORFORSERVER_H

#include "IProtocol.h"
#include "PackageParser.h"

class MessageProcessorForServer : public QObject
{
  Q_OBJECT

public:
  MessageProcessorForServer(ProtocolDataType protocol, log4cplus::Logger logger);
  ~MessageProcessorForServer();

  void parseMessage(QByteArray message, quintptr id);

signals:
  void appearedGeneratedArray(QByteArray message);

public:
  IProtocol* m_protocol = nullptr;
  PackageParser* m_parser = nullptr;
  quint32 m_id;
  log4cplus::Logger m_logger;
};

#endif // MESSAGEPROCESSORFORSERVER_H
