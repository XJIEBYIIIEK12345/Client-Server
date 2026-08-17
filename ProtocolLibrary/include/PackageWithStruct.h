#ifndef PACKAGEWITHSTRUCT_H
#define PACKAGEWITHSTRUCT_H

#include "Package.h"

struct BytesAndTypeToClient
{
  QString valueType;
  quint32 bytes;

  BytesAndTypeToClient()
    : valueType("qint32")
    , bytes(0)
  {}
  BytesAndTypeToClient(QString _valueType, quint32 _bytes)
    : valueType(_valueType)
    , bytes(_bytes)
  {}
};

class PackageWithStruct : public Package
{
  BytesAndTypeToClient m_clientStruct;
};

#endif // PACKAGEWITHSTRUCT_H
