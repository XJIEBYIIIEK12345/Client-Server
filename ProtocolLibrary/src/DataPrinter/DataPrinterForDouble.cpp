#include "DataPrinterForDouble.h"

DataPrinterForDouble::DataPrinterForDouble(log4cplus::Logger logger)
{
  m_logger = logger;
}

DataPrinterForDouble::~DataPrinterForDouble()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

void DataPrinterForDouble::printData(QVariantList data, qint32 clientId)
{
  QString str;
  for (int i = 0; i < data.size(); ++i)
  {
    str += QString::number(data[i].toDouble(), 'f', 6) + ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);
}
