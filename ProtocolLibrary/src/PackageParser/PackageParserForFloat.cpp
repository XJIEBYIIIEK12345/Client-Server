#include "PackageParserForFloat.h"

PackageParserForFloat::PackageParserForFloat(log4cplus::Logger logger)
{
  m_logger = logger;
}

PackageParserForFloat::~PackageParserForFloat() {}

void PackageParserForFloat::parseAndPrintPackage(QByteArray data, qint32 clientId)
{
  m_buffer.append(data);
  const float* sinus = reinterpret_cast<const float*>(m_buffer.constData());

  qint32 count = m_buffer.size() / sizeof(float);

  QString str;
  for (int i = 0; i < count; ++i)
  {
    str += QString::number(sinus[i], 'f', 6) + ", ";
  }

  LOG4CPLUS_TRACE(m_logger, "Received: " << str.toStdString() << " from Client "
                                         << clientId << "\n"
                                         << "\n";);
  LOG4CPLUS_INFO(m_logger, "Received data from Client " << clientId;);

  m_buffer.remove(0, count * sizeof(float));
}
