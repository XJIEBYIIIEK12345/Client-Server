#include "PackageParserForInt16.h"
#include <QDebug>

PackageParserForInt16::PackageParserForInt16() {}

PackageParserForInt16::~PackageParserForInt16() {}

void PackageParserForInt16::parsePackage(QByteArray data) {

    m_buffer.append(data);
    qint16* sinus = reinterpret_cast<qint16*>(m_buffer.data());

    qint32 count = m_buffer.size() / sizeof(qint16);
    m_parsedSinus = sinus;

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(m_parsedSinus[i]) + ", ";
    }

    qDebug() << "Server received:" << str;

    m_buffer.remove(0, count * sizeof(qint16));
}
