#ifndef JSONPROTOCOL_H
#define JSONPROTOCOL_H
#include "IProtocol.h"
#include <QJsonObject>

class JsonProtocol : public IProtocol {

public:
    JsonProtocol();
    JsonProtocol(qint32 count, const QString& type, const QByteArray& data);
    ~JsonProtocol();

    QByteArray encodeData() override;
    bool decodeData() override;

public:
    QJsonObject m_jsonObj;
};

#endif // JSONPROTOCOL_H
