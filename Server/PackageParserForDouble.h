#ifndef PACKAGEPARSERFORDOUBLE_H
#define PACKAGEPARSERFORDOUBLE_H
#include "PackageParser.h"

class PackageParserForDouble : public PackageParser {

public:
    PackageParserForDouble();
    ~PackageParserForDouble();

    void parsePackage(QByteArray data) override;
};

#endif // PACKAGEPARSERFORDOUBLE_H
