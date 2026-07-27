#ifndef PACKAGEPARSER_H
#define PACKAGEPARSER_H
#include <QObject>

enum class PackageParserType {

    PackageParserForInt16 = 0,
    PackageParserForInt32,
    PackageParserForInt64,
    PackageParserForFloat,
    PackageParserForDouble,
    Count
};

class PackageParser
{
public:
    virtual ~PackageParser() = default;
    virtual void parseAndPrintPackage(QByteArray data, qint32 clientId) = 0;

    static PackageParser* makeParser(PackageParserType type);


protected:
    QByteArray m_buffer;
};

QString packageParserTypeName (PackageParserType type);

#endif // PACKAGEPARSER_H
