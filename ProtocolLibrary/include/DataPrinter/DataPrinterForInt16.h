#ifndef DATAPRINTERFORINT16_H
#define DATAPRINTERFORINT16_H

#include "DataPrinter.h"

class DataPrinterForInt16 : public DataPrinter
{
public:
  DataPrinterForInt16(log4cplus::Logger logger);
  ~DataPrinterForInt16();

  void printData(QVariantList data, qint32 clientId) override;
};

#endif // DATAPRINTERFORINT16_H
