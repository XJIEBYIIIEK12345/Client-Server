#include "MessageProcessorForServer.h"
#include "Package.h"
#include "PackageForDataToGenerate.h"
#include "PackageForGeneratedData.h"
#include "PackageForSignal.h"
#include "PackageParser.h"
#include <QRandomGenerator64>

MessageProcessorForServer::MessageProcessorForServer(ProtocolDataType protocol,
                                                     log4cplus::Logger logger)
{
  m_logger = logger;
  m_protocol = IProtocol::makeProtocol(protocol, logger);
}

MessageProcessorForServer::~MessageProcessorForServer()
{
  if (m_protocol != nullptr)
  {
    delete m_protocol;
    m_protocol = nullptr;
  }
  if (m_parser != nullptr)
  {
    delete m_parser;
    m_parser = nullptr;
  }
}

void MessageProcessorForServer::parseMessage(QByteArray message, quintptr id)
{
  m_protocol->m_buffer.append(message);

  Package* packFromSender = m_protocol->decodeData();

  if (packFromSender != nullptr)
  {
    switch (packFromSender->m_type)
    {
    case MessageType::MetaDataRequest:
    {
      QRandomGenerator* randomGenerator = QRandomGenerator::global();

      PackageParserType packageParserType = PackageParserType(
          randomGenerator->bounded(0, int(PackageParserType::Count) - 1));

      QString valueType = packageParserTypeName(packageParserType);
      m_parser = PackageParser::makeParser(packageParserType, m_logger);

      quint32 bytes = randomGenerator->bounded(0, 100000);

      Package* pack = new PackageForDataToGenerate(
          packFromSender->m_id + 1, MessageType::MetaDataResponse, valueType, bytes);
      QByteArray messageFromReceiver = m_protocol->encodeData(pack);

      emit appearedGeneratedArray(messageFromReceiver);
    }
    break;
    case MessageType::SinAnswer:
    {
      Package* pack = new PackageForSignal(packFromSender->m_id + 1,
                                           MessageType::SinConfirmation, true);
      QByteArray messageFromReceiver = m_protocol->encodeData(pack);

      emit appearedGeneratedArray(messageFromReceiver);

      PackageForGeneratedData* dataPackFromSender =
          dynamic_cast<PackageForGeneratedData*>(packFromSender);

      m_parser->parseAndPrintPackage(dataPackFromSender->m_data, id);

      if (dataPackFromSender != nullptr)
      {
        dataPackFromSender = nullptr;
      }
    }
    break;
    default:
      LOG4CPLUS_WARN(m_logger, "Unable to process this type of message: "
                                   << packFromSender->m_type);
    }
  }
  if (packFromSender != nullptr)
  {
    delete packFromSender;
    packFromSender = nullptr;
  }
}
