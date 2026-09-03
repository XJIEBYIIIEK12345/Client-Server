#include "Client.h"
#include "MessageProcessorForClient.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <signal.h>

void closeClient(int sig)
{
  qDebug() << '\n' << "Client is closing...";
  QCoreApplication::exit(sig);
}

void connectClientToMessageProcessorForClient(Client& client,
                                              MessageProcessorForClient& processor)
{
  QObject::connect(&client, &Client::readyForSend, &processor,
                   &MessageProcessorForClient::makeDataRequestMessage);
  QObject::connect(&client, &Client::notReadyForSend, &processor,
                   &MessageProcessorForClient::stopGeneration);
  QObject::connect(&client, &Client::bytesReceived, &processor,
                   &MessageProcessorForClient::parseMessage);
  QObject::connect(&processor, &MessageProcessorForClient::appearedGeneratedArray,
                   &client, &Client::writeToServer);
  QObject::connect(&processor, &MessageProcessorForClient::needToConfirmArray,
                   &client, &Client::closeSocket);
}

int main(int argc, char* argv[])
{
  QCoreApplication a(argc, argv);
  signal(SIGINT, closeClient);

  log4cplus::Initializer initializer;
  log4cplus::PropertyConfigurator config("../clientLog.properties");
  config.configure();
  log4cplus::Logger logger =
      log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Client"));

  QMap<QString, ProtocolDataType> protocols = {{"json", ProtocolDataType::JsonType},
                                               {"bin", ProtocolDataType::BinType},
                                               {"xml", ProtocolDataType::XmlType}};

  QMap<QString, log4cplus::LogLevel> logLevels = {
      {"trace", log4cplus::TRACE_LOG_LEVEL}, {"debug", log4cplus::DEBUG_LOG_LEVEL},
      {"info", log4cplus::INFO_LOG_LEVEL},   {"warn", log4cplus::WARN_LOG_LEVEL},
      {"error", log4cplus::ERROR_LOG_LEVEL},
  };

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption addressOption(QStringList() << "address",
                                   "Server IP (default: 127.0.0.1)", "address",
                                   "127.0.0.1");
  parser.addOption(addressOption);
  QCommandLineOption portOption(QStringList() << "port", "Port (default: 6789)",
                                "port", "6789");
  parser.addOption(portOption);
  QCommandLineOption protocolOption(
      QStringList() << "protocol",
      "Transmitted data format (json, bin, xml) (default: json)", "protocol",
      "json");
  parser.addOption(protocolOption);
  QCommandLineOption logLevelOption(QStringList() << "loglevel",
                                    "Log level (default: info)", "loglevel", "info");
  parser.addOption(logLevelOption);

  parser.process(a);

  QString logLevel = parser.value(logLevelOption);
  logger.setLogLevel(logLevels[logLevel]);

  QString address = parser.value(addressOption);
  quint16 port = parser.value(portOption).toUShort();
  QString protocol = parser.value(protocolOption);

  if (!protocols.contains(protocol))
  {
    LOG4CPLUS_ERROR(logger, "This data format is not supported");
    return -1;
  }
  else
  {
    Client client(address, port, protocols[protocol], logger);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &client, &Client::closeAll);
    MessageProcessorForClient processor(protocols[protocol], logger);

    connectClientToMessageProcessorForClient(client, processor);
    client.connect();

    return a.exec();
  }
}
