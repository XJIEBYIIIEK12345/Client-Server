#include "PackageParserForInt16.h"
#include <QDebug>

PackageParserForInt16::PackageParserForInt16() {}

PackageParserForInt16::~PackageParserForInt16() {}

void PackageParserForInt16::parseAndPrintPackage(QByteArray data, qint32 clientId) {

    m_buffer.append(data);
    const qint16* sinus = reinterpret_cast<const qint16*>(m_buffer.constData());

    qint32 count = m_buffer.size() / sizeof(qint16);

    QString str;
    for (int i = 0; i < count; ++i) {
        str += QString::number(float(sinus[i]) / float(std::numeric_limits<qint16>::max())) + ", ";
    }

    qDebug() << "Server received:" << str << "from Client" << clientId << "\n" << "\n";

    m_buffer.remove(0, count * sizeof(qint16));
}
