#include "PackageParserForInt64.h"
#include <QDebug>

PackageParserForInt64::PackageParserForInt64() {}

PackageParserForInt64::~PackageParserForInt64() {}

void PackageParserForInt64::parsePackage(QByteArray data) {

    m_buffer.append(data);
    qint64* sinus = reinterpret_cast<qint64*>(m_buffer.data());

    qint32 count = m_buffer.size() / sizeof(qint64);
    m_parsedSinus = sinus;

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(m_parsedSinus[i]) + ", ";
    }

    qDebug() << "Server received:" << str;

    m_buffer.remove(0, count * sizeof(qint64));
}
