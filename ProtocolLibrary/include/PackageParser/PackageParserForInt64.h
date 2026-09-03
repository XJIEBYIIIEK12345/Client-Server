#ifndef PACKAGEPARSERFORINT64_H
#define PACKAGEPARSERFORINT64_H

#include "PackageParser.h"

class PackageParserForInt64 : public PackageParser
{
public:
  PackageParserForInt64(log4cplus::Logger logger);
  ~PackageParserForInt64();

  void parseAndPrintPackage(QByteArray data, qint32 clientId) override;
};

#endif // PACKAGEPARSERFORINT64_H
