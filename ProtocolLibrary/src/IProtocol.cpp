#include "IProtocol.h"
#include "JsonProtocol.h"

IProtocol* IProtocol::makeProtocol(ProtocolDataType type)
{
  switch (type)
  {
  case ProtocolDataType::JsonType:
    return new JsonProtocol();
  case ProtocolDataType::BinType:
    // return new BinProtocol();
  case ProtocolDataType::XmlType:
    // return new XmlProtocol();
  default:
    return new JsonProtocol();
  }
}

IProtocol* IProtocol::makeProtocol(QString type)
{
  if (type == "json")
    return new JsonProtocol();
  // else if (type == "bin")
  //     return new BinProtocol();
  // else if (type == "xml")
  //     return new XmlProtocol();
  else
    return new JsonProtocol();
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
