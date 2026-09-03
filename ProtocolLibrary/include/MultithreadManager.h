#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include <MessageProcessorForServer.h>

class MultithreadManager : public QObject
{
  Q_OBJECT

public:
  MultithreadManager(ProtocolDataType protocol, log4cplus::Logger logger);

  void clientConnectedToServer(quintptr socketDescriptor);

signals:
  void cleanAll();

private:
  void clean();

private:
  ProtocolDataType m_protocol;
  log4cplus::Logger m_logger;
};

#endif // MULTITHREADMANAGER_H
