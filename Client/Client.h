#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include "IProtocol.h"
#include "SineGenerator.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QTimer>

class Client : public QObject
{
  Q_OBJECT

public:
  explicit Client(QString address, quint16 port, ProtocolDataType protocol,
                  QObject* parent = nullptr);
  ~Client();
  void connect();

private slots:
  void initPackageSending();
  void startReconnectTimer();
  void readDataFromServer();
  void closeSocket();

protected:
  void sendPackageToServer();
  void connectToServer();
  void timerEvent(QTimerEvent* event) override;

private:
  quint16 m_port = 6789;
  QString m_address = "127.0.0.1";
  QTcpSocket* m_socket = nullptr;
  int m_timerId = 0;
  SineGenerator* m_generator = nullptr;
  IProtocol* m_protocol = nullptr;
};

#endif // CLIENT_H
