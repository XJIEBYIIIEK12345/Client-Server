#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include <MessageProcessorForServer.h>

class MultithreadManager : public QObject
{
  Q_OBJECT

public:
  MultithreadManager(ProtocolDataType protocol, log4cplus::Logger logger);
  ~MultithreadManager();

  void clientConnectedToServer(quintptr socketDescriptor);
  void setCountOfClientsOnDisconnect();

signals:
  void cleanAll();
  void countOfClientsChanged(qint32 countOfClients);

private:
  void clean();

private:
  ProtocolDataType m_protocol;
  log4cplus::Logger m_logger;
  qint32 m_countOfClients = 0;
};

#endif // MULTITHREADMANAGER_H
