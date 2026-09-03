#include "PackageParserForInt64.h"

PackageParserForInt64::PackageParserForInt64(log4cplus::Logger logger)
{
  m_logger = logger;
}

PackageParserForInt64::~PackageParserForInt64() {}

void PackageParserForInt64::parseAndPrintPackage(QByteArray data, qint32 clientId)
{
  m_buffer.append(data);
  const qint64* sinus = reinterpret_cast<const qint64*>(m_buffer.constData());

  qint32 count = m_buffer.size() / sizeof(qint64);

  QString str;
  for (int i = 0; i < count; ++i)
  {
    str += QString::number(float(sinus[i]) /
                           float(std::numeric_limits<qint64>::max())) +
           ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);

  m_buffer.remove(0, count * sizeof(qint64));
}
