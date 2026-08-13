#ifndef JSONPROTOCOL_H
#define JSONPROTOCOL_H

#include "IProtocol.h"
#include <QJsonObject>

class JsonProtocol : public IProtocol
{
public:
  JsonProtocol();
  ~JsonProtocol();

  QByteArray encodeData(Package* pack) override;
  Package* decodeData() override;
};

#endif // JSONPROTOCOL_H
