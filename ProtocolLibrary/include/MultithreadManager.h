#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include <MessageProcessorForServer.h>

class MultithreadManager : public QObject
{
  Q_OBJECT

public:
  MultithreadManager(ProtocolDataType protocol);

  void clientConnectedToServer(quintptr socketDescriptor);

signals:
  void cleanAll();

private:
  void clean();

private:
  ProtocolDataType m_protocol;
};

#endif // MULTITHREADMANAGER_H
