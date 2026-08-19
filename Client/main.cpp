#include "Client.h"
#include <QCommandLineParser>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
  QCoreApplication a(argc, argv);

  QMap<QString, ProtocolDataType> protocols = {{"json", ProtocolDataType::JsonType},
                                               {"bin", ProtocolDataType::BinType},
                                               {"xml", ProtocolDataType::XmlType}};

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

  parser.process(a);

  QString address = parser.value(addressOption);
  quint16 port = parser.value(portOption).toUShort();
  QString protocol = parser.value(protocolOption);

  if (!protocols.contains(protocol))
  {
    qDebug() << "This data format is not supported";
    return -1;
  }
  else
  {
    Client client(address, port, protocols[protocol]);
    client.connect();

    return a.exec();
  }
}
