#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include "IProtocol.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

class Client : public QObject
{
  Q_OBJECT

public:
  explicit Client(QString address, quint16 port, ProtocolDataType protocol,
                  log4cplus::Logger logger, QObject* parent = nullptr);
  ~Client();
  void connect();
  void closeAll();

signals:
  void readyForSend();
  void notReadyForSend();
  void bytesReceived(QByteArray message);

private slots:
  void stopReconnecting();
  void startReconnecting();
  void readDataFromServer();
  void logError();

public:
  void closeSocket();
  void connectToServer();
  void writeToServer(QByteArray message);
  void timerEvent(QTimerEvent* event) override;

private:
  quint16 m_port = 6789;
  QString m_address = "127.0.0.1";
  QTcpSocket* m_socket = nullptr;
  int m_timerIdForConnect = 0;
  log4cplus::Logger m_logger;
};

#endif // CLIENT_H
