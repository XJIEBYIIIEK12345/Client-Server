#ifndef IPROTOCOL_H
#define IPROTOCOL_H
#include <QByteArray>
#include <QString>
#include <QDebug>

enum class ProtocolDataType {
    JsonType = 0,
    BinType,
    XmlType,
    Count
};

struct Package {
    qint32 count;
    QString type;
    QByteArray data;

    Package() : count(0), type("") {}
    Package(qint32 _count, QString _type, QByteArray _data) : count(_count), type(_type), data(_data) {}
    Package(const Package& pack) {this->count = pack.count; this->type = pack.type; this->data = pack.data;}

    void setPackageData(qint32 _count, QString _type, QByteArray _data) {this->count = _count; this->type = _type; this->data = _data;}
    void setPackageData(const Package& pack) {this->count = pack.count; this->type = pack.type; this->data = pack.data;}
};

class IProtocol {
public:
    virtual ~IProtocol() = default;

    static IProtocol* makeProtocol(ProtocolDataType type);
    static IProtocol* makeProtocol(QString type);
    virtual QByteArray encodeData() = 0;
    virtual bool decodeData() = 0;

    QByteArray m_buffer;
    Package m_package;
};

#endif // IPROTOCOL_H
