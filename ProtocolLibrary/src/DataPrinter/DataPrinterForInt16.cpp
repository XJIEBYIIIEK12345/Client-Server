#include "DataPrinterForInt16.h"

DataPrinterForInt16::DataPrinterForInt16(log4cplus::Logger logger)
{
  m_logger = logger;
}

DataPrinterForInt16::~DataPrinterForInt16()
{
  LOG4CPLUS_INFO(m_logger, typeid(this).name() << " was destroyed\n");
}

void DataPrinterForInt16::printData(QVariantList data, qint32 clientId)
{
  QString str;
  for (int i = 0; i < data.size(); ++i)
  {
    str += QString::number(float(data[i].value<qint16>()) /
                           float(std::numeric_limits<qint16>::max())) +
           ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);
}
