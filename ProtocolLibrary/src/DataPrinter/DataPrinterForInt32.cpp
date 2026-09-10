#include "DataPrinterForInt32.h"

DataPrinterForInt32::DataPrinterForInt32(log4cplus::Logger logger)
{
  m_logger = logger;
}

DataPrinterForInt32::~DataPrinterForInt32()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

void DataPrinterForInt32::printData(QVariantList data, qint32 clientId)
{
  QString str;
  for (int i = 0; i < data.size(); ++i)
  {
    str += QString::number(float(data[i].toInt()) /
                           float(std::numeric_limits<qint32>::max())) +
           ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);
}
