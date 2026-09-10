#include "DataPrinter.h"
#include "DataPrinterForDouble.h"
#include "DataPrinterForFloat.h"
#include "DataPrinterForInt16.h"
#include "DataPrinterForInt32.h"
#include "DataPrinterForInt64.h"

DataPrinter::~DataPrinter()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

DataPrinter* DataPrinter::makePrinter(DataPrinterType type, log4cplus::Logger logger)
{
  switch (type)
  {
  case DataPrinterType::DataPrinterForInt16:
    return new DataPrinterForInt16(logger);
  case DataPrinterType::DataPrinterForInt32:
    return new DataPrinterForInt32(logger);
  case DataPrinterType::DataPrinterForInt64:
    return new DataPrinterForInt64(logger);
  case DataPrinterType::DataPrinterForFloat:
    return new DataPrinterForFloat(logger);
  case DataPrinterType::DataPrinterForDouble:
    return new DataPrinterForDouble(logger);
  default:
    return new DataPrinterForInt32(logger);
  }
}

QString dataPrinterTypeName(DataPrinterType type)
{
  switch (type)
  {
  case DataPrinterType::DataPrinterForInt16:
    return "qint16";
  case DataPrinterType::DataPrinterForInt32:
    return "qint32";
  case DataPrinterType::DataPrinterForInt64:
    return "qint64";
  case DataPrinterType::DataPrinterForFloat:
    return "float";
  case DataPrinterType::DataPrinterForDouble:
    return "double";
  default:
    return "qint32";
  }
}
