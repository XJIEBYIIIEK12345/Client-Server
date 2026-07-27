#ifndef PACKAGEPARSERFORINT16_H
#define PACKAGEPARSERFORINT16_H
#include "PackageParser.h"

class PackageParserForInt16 : public PackageParser {

public:
    PackageParserForInt16();
    ~PackageParserForInt16();

    void parsePackage(QByteArray data) override;
};

#endif // PACKAGEPARSERFORINT16_H
