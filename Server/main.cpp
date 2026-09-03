#include "MultithreadManager.h"
#include "Server.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMetaType>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <signal.h>

void closeServer(int sig)
{
  qDebug() << '\n' << "Server is closing...";

  QCoreApplication::exit(sig);
}

int main(int argc, char* argv[])
{
  QCoreApplication a(argc, argv);
  qRegisterMetaType<quintptr>("quintptr");
  signal(SIGINT, closeServer);

  log4cplus::Initializer initializer;
  log4cplus::PropertyConfigurator config("../serverLog.properties");
  config.configure();
  log4cplus::Logger logger =
      log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Server"));

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

  quint16 port = parser.value(portOption).toUShort();
  QString protocol = parser.value(protocolOption);

  if (!protocols.contains(protocol))
  {
    LOG4CPLUS_ERROR(logger, "This data format is not supported");
    return -1;
  }
  else
  {
    Server server(port, protocols[protocol], logger);
    MultithreadManager manager(protocols[protocol], logger);
    QObject::connect(&server, &Server::clientStartConnecting, &manager,
                     &MultithreadManager::clientConnectedToServer);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &manager,
                     &MultithreadManager::cleanAll);
    return a.exec();
  }
}
