#ifndef PACKAGEPARSERFORINT16_H
#define PACKAGEPARSERFORINT16_H
#include "PackageParser.h"

class PackageParserForInt16 : public PackageParser {

public:
    PackageParserForInt16();
    ~PackageParserForInt16();

    void parseAndPrintPackage(QByteArray data, qint32 clientId) override;
};

#endif // PACKAGEPARSERFORINT16_H
