#include "PackageParser.h"
#include "PackageParserForDouble.h"
#include "PackageParserForFloat.h"
#include "PackageParserForInt16.h"
#include "PackageParserForInt32.h"
#include "PackageParserForInt64.h"

PackageParser *PackageParser::makeParser(PackageParserType type) {

    switch (type) {
    case PackageParserType::PackageParserForInt16:
        return new PackageParserForInt16();
    case PackageParserType::PackageParserForInt32:
        return new PackageParserForInt32();
    case PackageParserType::PackageParserForInt64:
        return new PackageParserForInt64();
    case PackageParserType::PackageParserForFloat:
        return new PackageParserForFloat();
    case PackageParserType::PackageParserForDouble:
        return new PackageParserForDouble();
    default:
        return new PackageParserForInt32();
    }
}

QString packageParserTypeName(PackageParserType type) {

    switch (type) {
    case PackageParserType::PackageParserForInt16:
        return "qint16";
    case PackageParserType::PackageParserForInt32:
        return "qint32";
    case PackageParserType::PackageParserForInt64:
        return "qint64";
    case PackageParserType::PackageParserForFloat:
        return "float";
    case PackageParserType::PackageParserForDouble:
        return "double";
    default:
        return "qint32";
    }
}
