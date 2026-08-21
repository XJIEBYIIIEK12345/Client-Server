#ifndef MESSAGEPROCESSORFORCLIENT_H
#define MESSAGEPROCESSORFORCLIENT_H

#include "IProtocol.h"
#include "SineGenerator.h"

class MessageProcessorForClient : public QObject
{
  Q_OBJECT

public:
  MessageProcessorForClient(ProtocolDataType protocol);
  ~MessageProcessorForClient();

  void clientConnectedToServer();

  void parseMessage(QByteArray message);
  void generateMessageOnTimer();
  void stopSending();
  void startSending();

  void timerEvent(QTimerEvent* event) override;

signals:
  void appearedGeneratedArray(QByteArray message);
  void waitForConfirmation();

public:
  int m_timerIdForSend = 0;
  SineGenerator* m_generator = nullptr;
  IProtocol* m_protocol = nullptr;
};

#endif // MESSAGEPROCESSORFORCLIENT_H
