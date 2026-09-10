#ifndef PACKAGEPARSERFORINT32_H
#define PACKAGEPARSERFORINT32_H

#include "PackageParser.h"

class PackageParserForInt32 : public PackageParser
{
public:
  PackageParserForInt32(log4cplus::Logger logger);
  ~PackageParserForInt32();

  void parseAndPrintPackage(QByteArray data, qint32 clientId) override;
};

#endif // PACKAGEPARSERFORINT32_H
