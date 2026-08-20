#ifndef PACKAGEPARSERFORDOUBLE_H
#define PACKAGEPARSERFORDOUBLE_H

#include "PackageParser.h"

class PackageParserForDouble : public PackageParser
{
public:
  PackageParserForDouble();
  ~PackageParserForDouble();

  void parseAndPrintPackage(QByteArray data, qint32 clientId) override;
};

#endif // PACKAGEPARSERFORDOUBLE_H
