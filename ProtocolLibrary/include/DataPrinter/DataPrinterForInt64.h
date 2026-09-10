#ifndef DATAPRINTERFORINT64_H
#define DATAPRINTERFORINT64_H

#include "DataPrinter.h"

class DataPrinterForInt64 : public DataPrinter
{
public:
  DataPrinterForInt64(log4cplus::Logger logger);
  ~DataPrinterForInt64();

  void printData(QVariantList data, qint32 clientId) override;
};

#endif // DATAPRINTERFORINT64_H
