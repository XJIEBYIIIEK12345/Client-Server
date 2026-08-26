#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QTcpSocket>

class WorkerThread : public QObject
{
  Q_OBJECT

public:
  WorkerThread(quintptr socketDescriptor = 0);

  void writeToReceiver(QByteArray message);

  void process();
  void stop();

signals:
  void bytesReceived(QByteArray message, quintptr socketDescriptor);
  void wasDisconnected();

private slots:
  void readDataFromSender();
  void logErrorAndCloseSocket();
  void logClientState();

private:
  quintptr m_socketDescriptor;
  QTcpSocket* m_socket = nullptr;
};

#endif // WORKERTHREAD_H
