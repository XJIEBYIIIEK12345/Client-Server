#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

class WorkerThread : public QObject
{
  Q_OBJECT

public:
  WorkerThread(log4cplus::Logger logger, quintptr socketDescriptor = 0);

  void writeToReceiver(QByteArray message);
  quintptr getSocketDescriptor();

  void process();
  void stop();

signals:
  void bytesReceived(QByteArray message, quintptr socketDescriptor);
  void finished();

private slots:
  void readDataFromSender();
  void logErrorAndCloseSocket();
  void logClientState();

private:
  quintptr m_socketDescriptor;
  QTcpSocket* m_socket = nullptr;
  log4cplus::Logger m_logger;
};

#endif // WORKERTHREAD_H
