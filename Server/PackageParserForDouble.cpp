#include "PackageParserForDouble.h"
#include <QDebug>

PackageParserForDouble::PackageParserForDouble() {}

PackageParserForDouble::~PackageParserForDouble() {}

void PackageParserForDouble::parsePackage(QByteArray data) {

    m_buffer.append(data);
    double* sinus = reinterpret_cast<double*>(m_buffer.data());

    qint32 count = m_buffer.size() / sizeof(double);

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(sinus[i], 'f', 6) + ", ";
    }

    qDebug() << "Server received:" << str;

    m_buffer.remove(0, count * sizeof(double));
}
