#include "MessageProcessorForClient.h"
#include "Package.h"
#include "PackageDataArray.h"
#include "PackageMetaData.h"
#include <QTimerEvent>

MessageProcessorForClient::MessageProcessorForClient(ProtocolDataType protocol,
                                                     log4cplus::Logger logger)
{
  m_protocol = IProtocol::makeProtocol(protocol, logger);
  m_logger = logger;
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

  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

void MessageProcessorForClient::makeDataRequestMessage()
{
  Package* pack = new Package(0, MessageType::MetaDataRequest);

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
      PackageMetaData* packageFromSender = dynamic_cast<PackageMetaData*>(pack);

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
      LOG4CPLUS_WARN(m_logger,
                     "Unable to process this type of message: " << pack->m_type);
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
  QVariantList data = m_generator->generateSineForType();

  Package* pack = new PackageDataArray(1, data);
  QByteArray message = m_protocol->encodeData(pack);

  LOG4CPLUS_INFO(m_logger, "This send data");

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
