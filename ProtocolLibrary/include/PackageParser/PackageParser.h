#ifndef PACKAGEPARSER_H
#define PACKAGEPARSER_H

#include <QObject>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

enum class PackageParserType
{
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

  static PackageParser* makeParser(PackageParserType type, log4cplus::Logger logger);

protected:
  QByteArray m_buffer;
  log4cplus::Logger m_logger;
};

QString packageParserTypeName(PackageParserType type);

#endif // PACKAGEPARSER_H
