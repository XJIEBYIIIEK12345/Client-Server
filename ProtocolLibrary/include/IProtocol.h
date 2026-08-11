#ifndef IPROTOCOL_H
#define IPROTOCOL_H
#include <QByteArray>

enum class ProtocolDataType {
    JsonType = 0,
    BinType,
    XmlType
};

class IProtocol {
public:
    virtual ~IProtocol() = default;

    static IProtocol* makeProtocol(ProtocolDataType type);
    virtual QByteArray encodeData();
    virtual bool decodeData();

    QByteArray m_buffer;
};

#endif // IPROTOCOL_H
