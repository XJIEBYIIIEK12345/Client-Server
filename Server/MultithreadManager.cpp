#include "MultithreadManager.h"
#include "WorkerThread.h"
#include <QThread>

MultithreadManager::MultithreadManager(ProtocolDataType protocol,
                                       log4cplus::Logger logger)
{
  m_logger = logger;
  m_protocol = protocol;
}

MultithreadManager::~MultithreadManager()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

void MultithreadManager::setCountOfClientsOnDisconnect()
{
  m_countOfClients -= 1;
  emit countOfClientsChanged(m_countOfClients);
}

void MultithreadManager::clientConnectedToServer(quintptr socketDescriptor)
{
  WorkerThread* worker = new WorkerThread(m_logger, socketDescriptor);
  QThread* thread = new QThread;
  worker->moveToThread(thread);
  MessageProcessorForServer* processor =
      new MessageProcessorForServer(m_protocol, m_logger);

  QObject::connect(worker, &WorkerThread::bytesReceived, processor,
                   &MessageProcessorForServer::parseMessage);
  QObject::connect(this, &MultithreadManager::countOfClientsChanged, processor,
                   &MessageProcessorForServer::setCountOfClientsFromManager);
  QObject::connect(processor, &MessageProcessorForServer::appearedGeneratedArray,
                   worker, &WorkerThread::writeToReceiver);
  QObject::connect(this, &MultithreadManager::cleanAll, worker, &WorkerThread::stop);

  QObject::connect(thread, &QThread::started, worker, &WorkerThread::process);
  QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
  QObject::connect(worker, &WorkerThread::finished, worker, &QObject::deleteLater);
  QObject::connect(worker, &WorkerThread::finished, processor,
                   &QObject::deleteLater);
  QObject::connect(worker, &WorkerThread::finished, this,
                   &MultithreadManager::setCountOfClientsOnDisconnect);
  QObject::connect(worker, &QObject::destroyed, thread, &QThread::quit);

  m_countOfClients += 1;
  emit countOfClientsChanged(m_countOfClients);

  thread->start();
}
