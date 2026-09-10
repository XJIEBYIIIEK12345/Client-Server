#ifndef DATAPRINTERFORDOUBLE_H
#define DATAPRINTERFORDOUBLE_H

#include "DataPrinter.h"

class DataPrinterForDouble : public DataPrinter
{
public:
  DataPrinterForDouble(log4cplus::Logger logger);
  ~DataPrinterForDouble();

  void printData(QVariantList data, qint32 clientId) override;
};

#endif // DATAPRINTERFORDOUBLE_H
