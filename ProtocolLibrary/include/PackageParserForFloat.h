#ifndef PACKAGEPARSERFORFLOAT_H
#define PACKAGEPARSERFORFLOAT_H
#include "PackageParser.h"

class PackageParserForFloat : public PackageParser {

public:
    PackageParserForFloat();
    ~PackageParserForFloat();

    void parsePackage(QByteArray data) override;
};

#endif // PACKAGEPARSERFORFLOAT_H
