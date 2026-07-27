#include "PackageParserForFloat.h"
#include <QDebug>

PackageParserForFloat::PackageParserForFloat() {}

PackageParserForFloat::~PackageParserForFloat() {}

void PackageParserForFloat::parseAndPrintPackage(QByteArray data, qint32 clientId) {

    m_buffer.append(data);
    float* sinus = reinterpret_cast<float*>(m_buffer.data());

    qint32 count = m_buffer.size() / sizeof(float);

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(sinus[i], 'f', 6) + ", ";
    }

    qDebug() << "Server received:" << str << "from Client" << clientId << "\n" << "\n";

    m_buffer.remove(0, count * sizeof(float));
}
