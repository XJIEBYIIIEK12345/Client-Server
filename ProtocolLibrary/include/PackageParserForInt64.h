#ifndef PACKAGEPARSERFORINT64_H
#define PACKAGEPARSERFORINT64_H
#include "PackageParser.h"

class PackageParserForInt64 : public PackageParser {

public:
    PackageParserForInt64();
    ~PackageParserForInt64();

    void parsePackage(QByteArray data) override;
};

#endif // PACKAGEPARSERFORINT64_H
