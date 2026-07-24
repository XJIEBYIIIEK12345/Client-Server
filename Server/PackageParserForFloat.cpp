#include "PackageParserForFloat.h"
#include <QDebug>

PackageParserForFloat::PackageParserForFloat() {}

PackageParserForFloat::~PackageParserForFloat() {}

void PackageParserForFloat::parsePackage(QByteArray data) {

    m_buffer.append(data);
    float* sinus = reinterpret_cast<float*>(m_buffer.data());

    qint32 count = m_buffer.size() / sizeof(float);

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(sinus[i], 'f', 6) + ", ";
    }

    qDebug() << "Server received:" << str;

    m_buffer.remove(0, count * sizeof(float));
}
