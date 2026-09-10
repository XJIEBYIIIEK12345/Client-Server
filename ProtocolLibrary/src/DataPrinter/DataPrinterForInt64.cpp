#include "DataPrinterForInt64.h"

DataPrinterForInt64::DataPrinterForInt64(log4cplus::Logger logger)
{
  m_logger = logger;
}

DataPrinterForInt64::~DataPrinterForInt64()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

void DataPrinterForInt64::printData(QVariantList data, qint32 clientId)
{
  QString str;
  for (int i = 0; i < data.size(); ++i)
  {
    str += QString::number(float(data[i].toLongLong()) /
                           float(std::numeric_limits<qint64>::max())) +
           ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);
}
