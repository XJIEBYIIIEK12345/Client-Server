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
  if (m_protocol != nullptr)
  {
    delete m_protocol;
    m_protocol = nullptr;
  }
}

void MessageProcessorForClient::makeDataRequestMessage()
{
  Package* pack = new PackageForSignal(0, MessageType::MetaDataRequest, true);
  QByteArray message = m_protocol->encodeData(pack);

  emit appearedGeneratedArray(message);
  if (pack != nullptr)
  {
    delete pack;
    pack = nullptr;
  }
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
      PackageForDataToGenerate* packageFromSender =
          dynamic_cast<PackageForDataToGenerate*>(pack);

      m_generator = SineGenerator::makeGenerator(packageFromSender->m_valueType);
      m_generator->setCountOfBytes(packageFromSender->m_bytes);

      if (m_timerIdForSend == 0)
      {
        m_timerIdForSend = startTimer(2500);
      }
      if (packageFromSender != nullptr)
      {
        packageFromSender = nullptr;
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

void MessageProcessorForClient::generateMessage()
{
  QByteArray data = m_generator->generateSineForType();

  Package* pack = new PackageForGeneratedData(1, MessageType::SinAnswer, data);
  QByteArray message = m_protocol->encodeData(pack);

  qDebug() << "This send:" << data;

  emit appearedGeneratedArray(message);
  emit needToConfirmArray();
  if (pack != nullptr)
  {
    delete pack;
    pack = nullptr;
  }
}

void MessageProcessorForClient::stopGeneration()
{
  if (m_timerIdForSend != 0)
  {
    killTimer(m_timerIdForSend);
    m_timerIdForSend = 0;
  }
}
void MessageProcessorForClient::startGeneretion()
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
    generateMessage();
  }
  else
  {
    QObject::timerEvent(event);
  }
}
