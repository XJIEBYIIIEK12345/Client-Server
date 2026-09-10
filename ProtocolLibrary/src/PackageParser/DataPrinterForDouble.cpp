#include "PackageParserForDouble.h"

PackageParserForDouble::PackageParserForDouble(log4cplus::Logger logger)
{
  m_logger = logger;
}

PackageParserForDouble::~PackageParserForDouble() {}

void PackageParserForDouble::parseAndPrintPackage(QByteArray data, qint32 clientId)
{
  m_buffer.append(data);
  const double* sinus = reinterpret_cast<const double*>(m_buffer.constData());

  qint32 count = m_buffer.size() / sizeof(double);

  QString str;
  for (int i = 0; i < count; ++i)
  {
    str += QString::number(sinus[i], 'f', 6) + ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);

  m_buffer.remove(0, count * sizeof(double));
}
