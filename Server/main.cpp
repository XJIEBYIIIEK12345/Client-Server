#include "MultithreadManager.h"
#include "Server.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMetaType>
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

  QMap<QString, ProtocolDataType> protocols = {{"json", ProtocolDataType::JsonType},
                                               {"bin", ProtocolDataType::BinType},
                                               {"xml", ProtocolDataType::XmlType}};

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

  parser.process(a);

  quint16 port = parser.value(portOption).toUShort();
  QString protocol = parser.value(protocolOption);

  if (!protocols.contains(protocol))
  {
    qDebug() << "This data format is not supported";
    return -1;
  }
  else
  {
    Server server(port, protocols[protocol]);
    MultithreadManager manager(protocols[protocol]);
    QObject::connect(&server, &Server::clientStartConnecting, &manager,
                     &MultithreadManager::clientConnectedToServer);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &manager,
                     &MultithreadManager::cleanAll);
    return a.exec();
  }
}
