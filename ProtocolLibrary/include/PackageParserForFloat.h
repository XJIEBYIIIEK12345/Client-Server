#ifndef PACKAGEPARSERFORFLOAT_H
#define PACKAGEPARSERFORFLOAT_H

#include "PackageParser.h"

class PackageParserForFloat : public PackageParser
{
public:
  PackageParserForFloat();
  ~PackageParserForFloat();

  void parseAndPrintPackage(QByteArray data, qint32 clientId) override;
};

#endif // PACKAGEPARSERFORFLOAT_H
