#ifndef PACKAGEPARSERFORINT32_H
#define PACKAGEPARSERFORINT32_H
#include "PackageParser.h"

class PackageParserForInt32 : public PackageParser {

public:
    PackageParserForInt32();
    ~PackageParserForInt32();

    void parsePackage(QByteArray data) override;

    qint32* m_parsedSinus;
};

#endif // PACKAGEPARSERFORINT32_H
