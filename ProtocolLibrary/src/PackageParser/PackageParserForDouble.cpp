#include "PackageParserForDouble.h"
#include <QDebug>

PackageParserForDouble::PackageParserForDouble() {}

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

  qDebug() << "Server received:" << str << "from Client" << clientId << "\n"
           << "\n";

  m_buffer.remove(0, count * sizeof(double));
}
