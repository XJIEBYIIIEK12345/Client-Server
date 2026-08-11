#include "IProtocol.h"
#include "JsonProtocol.h"

IProtocol* IProtocol::makeProtocol(ProtocolDataType type) {

    switch(type) {
    case ProtocolDataType::JsonType:
        return new JsonProtocol();
    case ProtocolDataType::BinType:
        //return new BinProtocol();
    case ProtocolDataType::XmlType:
        //return new XmlProtocol();
    default:
        return new JsonProtocol();
    }
}
