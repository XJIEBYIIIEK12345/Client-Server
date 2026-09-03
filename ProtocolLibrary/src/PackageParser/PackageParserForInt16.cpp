#include "PackageParserForInt16.h"

PackageParserForInt16::PackageParserForInt16(log4cplus::Logger logger)
{
  m_logger = logger;
}

PackageParserForInt16::~PackageParserForInt16() {}

void PackageParserForInt16::parseAndPrintPackage(QByteArray data, qint32 clientId)
{
  m_buffer.append(data);
  const qint16* sinus = reinterpret_cast<const qint16*>(m_buffer.constData());

  qint32 count = m_buffer.size() / sizeof(qint16);

  QString str;
  for (int i = 0; i < count; ++i)
  {
    str += QString::number(float(sinus[i]) /
                           float(std::numeric_limits<qint16>::max())) +
           ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);

  m_buffer.remove(0, count * sizeof(qint16));
}
