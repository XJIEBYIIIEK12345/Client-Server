#include "PackageParserForInt16.h"
#include <QDebug>

PackageParserForInt16::PackageParserForInt16() {}

PackageParserForInt16::~PackageParserForInt16() {}

void PackageParserForInt16::parsePackage(QByteArray data) {

    m_buffer.append(data);
    qint16* sinus = reinterpret_cast<qint16*>(m_buffer.data());

    qint32 count = m_buffer.size() / sizeof(qint16);

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(float(sinus[i]) / float(std::numeric_limits<qint32>::max())) + ", ";
    }

    qDebug() << "Server received:" << str;

    m_buffer.remove(0, count * sizeof(qint16));
}
