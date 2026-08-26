#include "MultithreadManager.h"
#include <QThread>

MultithreadManager::MultithreadManager(ProtocolDataType protocol)
{
  m_protocol = protocol;
}

void MultithreadManager::connectClientToServer(quintptr socketDescriptor)
{
  WorkerThread* worker = new WorkerThread(socketDescriptor);
  QThread* thread = new QThread;
  MessageProcessorForServer* processor = new MessageProcessorForServer(m_protocol);
  // m_connectedClients.insert(socketDescriptor, {worker, processor});

  QObject::connect(worker, &WorkerThread::bytesReceived, processor,
                   &MessageProcessorForServer::parseMessage);
  QObject::connect(processor, &MessageProcessorForServer::appearedGeneratedArray,
                   worker, &WorkerThread::writeToReceiver);
  QObject::connect(worker, &WorkerThread::wasDisconnected, this,
                   &MultithreadManager::cleanClientData);

  QObject::connect(thread, &QThread::started, worker, &WorkerThread::process);
  QObject::connect(thread, &QThread::finished, worker, &WorkerThread::stop);

  worker->moveToThread(thread);
  thread->start();
}

void MultithreadManager::cleanClientData()
{
  // quintptr disconnectedSocket =
  //     qobject_cast<QTcpSocket*>(sender())->socketDescriptor();
  // if (!disconnectedSocket)
  //   return;

  // m_connectedClients.remove(disconnectedSocket);
}
