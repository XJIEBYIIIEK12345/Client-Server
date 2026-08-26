#ifndef MULTITHREADMANAGER_H
#define MULTITHREADMANAGER_H

#include "WorkerThread.h"
#include <MessageProcessorForServer.h>

class MultithreadManager : public QObject
{
  Q_OBJECT

public:
  MultithreadManager(ProtocolDataType protocol);

  void connectClientToServer(quintptr socketDescriptor);

private slots:
  void cleanClientData();

private:
  QMap<quintptr, std::pair<WorkerThread*, MessageProcessorForServer*>>
      m_connectedClients;
  ProtocolDataType m_protocol;
};

#endif // MULTITHREADMANAGER_H
