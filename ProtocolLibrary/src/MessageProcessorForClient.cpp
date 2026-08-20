#include "MessageProcessorForClient.h"
#include "Package.h"
#include "PackageForDataToGenerate.h"
#include "PackageForGeneratedData.h"
#include "PackageForSignal.h"
#include <QDebug>
#include <QTimerEvent>

MessageProcessorForClient::MessageProcessorForClient(ProtocolDataType protocol)
{
  m_protocol = IProtocol::makeProtocol(protocol);
}

MessageProcessorForClient::~MessageProcessorForClient()
{
  if (m_generator != nullptr)
  {
    delete m_generator;
    m_generator = nullptr;
  }
}

void MessageProcessorForClient::clientConnectedToServer()
{
  qDebug() << "Connected to server";

  Package* pack = new PackageForSignal(0, MessageType::MetaDataRequest, true);
  QByteArray message = m_protocol->encodeData(pack);

  emit readyToSend(message);
}

void MessageProcessorForClient::parseMessage(QByteArray message)
{
  m_protocol->m_buffer.append(message);

  Package* pack = m_protocol->decodeData();

  if (pack != nullptr)
  {
    switch (pack->m_type)
    {
    case MessageType::MetaDataResponse:
    {
      PackageForDataToGenerate* packageFromServer =
          dynamic_cast<PackageForDataToGenerate*>(pack);

      m_generator = SineGenerator::makeGenerator(packageFromServer->m_valueType);
      m_generator->setCountOfBytes(packageFromServer->m_bytes);

      if (m_timerIdForSend == 0)
      {
        m_timerIdForSend = startTimer(2500);
      }
    }
    break;
    case MessageType::SinConfirmation:
    {
      if (m_timerIdForSend == 0)
      {
        m_timerIdForSend = startTimer(2500);
      }
    }
    break;
    default:
      qDebug() << "Unable to process this type of message:" << pack->m_type;
    }
  }
  if (pack != nullptr)
  {
    delete pack;
    pack = nullptr;
  }
}

void MessageProcessorForClient::generateMessageOnTimer()
{
  QByteArray data = m_generator->generateSineForType();

  Package* pack = new PackageForGeneratedData(1, MessageType::SinAnswer, data);
  QByteArray message = m_protocol->encodeData(pack);

  qDebug() << "This send" << data;

  emit readyToSend(message);
}

void MessageProcessorForClient::stopSending()
{
  if (m_timerIdForSend != 0)
  {
    killTimer(m_timerIdForSend);
    m_timerIdForSend = 0;
  }
}
void MessageProcessorForClient::startSending()
{
  if (m_timerIdForSend == 0)
  {
    m_timerIdForSend = startTimer(2500);
  }
}

void MessageProcessorForClient::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == m_timerIdForSend)
  {
    generateMessageOnTimer();
  }
  else
  {
    QObject::timerEvent(event);
  }
}
