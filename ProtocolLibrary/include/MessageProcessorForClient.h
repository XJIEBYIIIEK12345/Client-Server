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

  void makeDataRequestMessage();

  void parseMessage(QByteArray message);
  void generateMessage();
  void stopGeneration();
  void startGeneretion();

  void timerEvent(QTimerEvent* event) override;

signals:
  void appearedGeneratedArray(QByteArray message);
  void needToConfirmArray();

public:
  int m_timerIdForSend = 0;
  SineGenerator* m_generator = nullptr;
  IProtocol* m_protocol = nullptr;
};

#endif // MESSAGEPROCESSORFORCLIENT_H
