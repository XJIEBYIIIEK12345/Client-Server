#include "MultithreadManager.h"
#include "WorkerThread.h"
#include <QThread>

MultithreadManager::MultithreadManager(ProtocolDataType protocol,
                                       log4cplus::Logger logger)
{
  m_logger = logger;
  m_protocol = protocol;
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
  QObject::connect(processor, &MessageProcessorForServer::appearedGeneratedArray,
                   worker, &WorkerThread::writeToReceiver);
  QObject::connect(this, &MultithreadManager::cleanAll, worker, &WorkerThread::stop);

  QObject::connect(thread, &QThread::started, worker, &WorkerThread::process);
  QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
  QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
  QObject::connect(worker, &WorkerThread::finished, thread, &QThread::quit);

  thread->start();
}
