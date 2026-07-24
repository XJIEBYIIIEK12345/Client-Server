#include "PackageParserForInt32.h"
#include <QDebug>

PackageParserForInt32::PackageParserForInt32() {}

PackageParserForInt32::~PackageParserForInt32() {}

void PackageParserForInt32::parsePackage(QByteArray data) {

    m_buffer.append(data);
    qint32* sinus = reinterpret_cast<qint32*>(m_buffer.data());

    qint32 count = m_buffer.size() / sizeof(qint32);

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(float(sinus[i]) / float(std::numeric_limits<qint32>::max())) + ", ";
    }

    qDebug() << "Server received:" << str;

    m_buffer.remove(0, count * sizeof(qint32));
}
