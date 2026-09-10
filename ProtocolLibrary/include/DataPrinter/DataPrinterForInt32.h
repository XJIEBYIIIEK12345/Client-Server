#ifndef DATAPRINTERFORINT32_H
#define DATAPRINTERFORINT32_H

#include "DataPrinter.h"

class DataPrinterForInt32 : public DataPrinter
{
public:
  DataPrinterForInt32(log4cplus::Logger logger);
  ~DataPrinterForInt32();

  void printData(QVariantList data, qint32 clientId) override;
};

#endif // DATAPRINTERFORINT32_H
