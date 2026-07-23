#ifndef PACKAGEPARSER_H
#define PACKAGEPARSER_H
#include <QObject>

class PackageParser
{
public:
    virtual ~PackageParser() = default;
    virtual void parsePackage(QByteArray data) = 0;

protected:
    QByteArray m_buffer;
};

#endif // PACKAGEPARSER_H
