#ifndef PACKAGEPARSERFORINT16_H
#define PACKAGEPARSERFORINT16_H
#include "PackageParser.h"

class PackageParserForInt16 : public PackageParser {

public:
    PackageParserForInt16();
    ~PackageParserForInt16();

    void parsePackage(QByteArray data) override;

    qint16* m_parsedSinus;
};

#endif // PACKAGEPARSERFORINT16_H
