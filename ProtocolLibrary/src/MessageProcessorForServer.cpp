#include "MessageProcessorForServer.h"
#include "DataPrinter.h"
#include "Package.h"
#include "PackageDataArray.h"
#include "PackageMetaData.h"
#include <QFile>
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
  if (m_printer != nullptr)
  {
    delete m_printer;
    m_printer = nullptr;
  }

  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

void MessageProcessorForServer::setCountOfClientsFromManager(qint32 count)
{
  m_countOfClientsFromManager = count;
}

bool MessageProcessorForServer::isHttpRequest(QByteArray message)
{
  if (message.startsWith("GET "))
    return true;
  else
    return false;
}

void MessageProcessorForServer::processHttpRequest(QByteArray message)
{
  int index = message.indexOf("GET /");

  if (index < 0)
    LOG4CPLUS_ERROR(m_logger, "Error to parse HTTP request");
  else
  {
    emit needCountOfClients();

    QByteArray response;
    QByteArray status;
    QByteArray html;

    QFile file("Server/page.html");
    if (file.open(QIODevice::ReadOnly))
    {
      QString htmlStr = QString::fromUtf8(file.readAll());
      file.close();
      status = "200 OK";
      html = htmlStr.arg(m_countOfClientsFromManager - 1).toUtf8();
    }
    else
    {
      status = "404 Not Found";
    }
    response.append("HTTP/1.1 " + status + "\r\n");
    response.append("Content-Type: text/html\r\n");
    response.append("Content-Length: " + QByteArray::number(html.size()) + "\r\n");
    response.append("Connection: close\r\n");
    response.append("\r\n");
    response.append(html);

    LOG4CPLUS_TRACE(m_logger, response.toStdString());

    emit appearedGeneratedArray(response);
  }
}

void MessageProcessorForServer::parseMessage(QByteArray message, quintptr id)
{
  if (isHttpRequest(message))
  {
    processHttpRequest(message);
  }
  else
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

        DataPrinterType dataPrinterType = DataPrinterType(
            randomGenerator->bounded(0, int(DataPrinterType::Count) - 1));

        QString valueType = dataPrinterTypeName(dataPrinterType);
        m_printer = DataPrinter::makePrinter(dataPrinterType, m_logger);

        quint32 bytes = randomGenerator->bounded(0, 100000);

        Package* pack =
            new PackageMetaData(packFromSender->m_id + 1, valueType, bytes);
        QByteArray messageFromReceiver = m_protocol->encodeData(pack);

        emit appearedGeneratedArray(messageFromReceiver);

        if (pack != nullptr)
        {
          delete pack;
          pack = nullptr;
        }
      }
      break;
      case MessageType::SinAnswer:
      {
        Package* pack =
            new Package(packFromSender->m_id + 1, MessageType::SinConfirmation);

        QByteArray messageFromReceiver = m_protocol->encodeData(pack);

        emit appearedGeneratedArray(messageFromReceiver);

        PackageDataArray* dataPackFromSender =
            dynamic_cast<PackageDataArray*>(packFromSender);

        m_printer->printData(dataPackFromSender->m_data, id);

        if (pack != nullptr)
        {
          delete pack;
          pack = nullptr;
        }
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
}
