#ifndef MESSAGEPROCESSORFORSERVER_H
#define MESSAGEPROCESSORFORSERVER_H

#include "IProtocol.h"
#include "DataPrinter.h"

class MessageProcessorForServer : public QObject
{
  Q_OBJECT

public:
  MessageProcessorForServer(ProtocolDataType protocol, log4cplus::Logger logger);
  ~MessageProcessorForServer();

  void parseMessage(QByteArray message, quintptr id);
  void setCountOfClientsFromManager(qint32 count);

private:
  bool isHttpRequest(QByteArray message);
  void processHttpRequest(QByteArray message);

signals:
  void appearedGeneratedArray(QByteArray message);
  void needCountOfClients();

private:
  IProtocol* m_protocol = nullptr;
  DataPrinter* m_printer = nullptr;
  quint32 m_id;
  log4cplus::Logger m_logger;
  qint32 m_countOfClientsFromManager;
};

#endif // MESSAGEPROCESSORFORSERVER_H
