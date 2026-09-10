#ifndef DATAPRINTER_H
#define DATAPRINTER_H

#include <QObject>
#include <QVariant>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

enum class DataPrinterType
{
  DataPrinterForInt16 = 0,
  DataPrinterForInt32,
  DataPrinterForInt64,
  DataPrinterForFloat,
  DataPrinterForDouble,
  Count
};

class DataPrinter
{
public:
  virtual ~DataPrinter();
  virtual void printData(QVariantList data, qint32 clientId) = 0;

  static DataPrinter* makePrinter(DataPrinterType type, log4cplus::Logger logger);

protected:
  log4cplus::Logger m_logger;
};

QString dataPrinterTypeName(DataPrinterType type);

#endif // DATAPRINTER_H
