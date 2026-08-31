#ifndef XMLPROTOCOL_H
#define XMLPROTOCOL_H

#include "IProtocol.h"

class XmlProtocol : public IProtocol
{
public:
  XmlProtocol();
  ~XmlProtocol();

  QByteArray encodeData(Package* pack) override;
  Package* decodeData() override;
};

#endif // XMLPROTOCOL_H
