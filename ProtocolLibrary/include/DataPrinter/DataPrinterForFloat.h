#ifndef DATAPRINTERFORFLOAT_H
#define DATAPRINTERFORFLOAT_H

#include "DataPrinter.h"

class DataPrinterForFloat : public DataPrinter
{
public:
  DataPrinterForFloat(log4cplus::Logger logger);
  ~DataPrinterForFloat();

  void printData(QVariantList data, qint32 clientId) override;
};

#endif // DATAPRINTERFORFLOAT_H
