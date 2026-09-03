#include "IProtocol.h"
#include "JsonProtocol.h"
#include "XmlProtocol.h"

IProtocol* IProtocol::makeProtocol(ProtocolDataType type, log4cplus::Logger logger)
{
  switch (type)
  {
  case ProtocolDataType::JsonType:
    return new JsonProtocol(logger);
  case ProtocolDataType::BinType:
    // return new BinProtocol(logger);
  case ProtocolDataType::XmlType:
    return new XmlProtocol(logger);
  default:
    return new JsonProtocol(logger);
  }
}

IProtocol* IProtocol::makeProtocol(QString type, log4cplus::Logger logger)
{
  if (type == "json")
    return new JsonProtocol(logger);
  // else if (type == "bin")
  //     return new BinProtocol(logger);
  else if (type == "xml")
    return new XmlProtocol(logger);
  else
    return new JsonProtocol(logger);
}

QString IProtocol::toString(ProtocolDataType type)
{
  switch (type)
  {
  case ProtocolDataType::JsonType:
    return "json";
  case ProtocolDataType::BinType:
    return "bin";
  case ProtocolDataType::XmlType:
    return "xml";
  default:
    return "Unsupported protocol";
  }
}
