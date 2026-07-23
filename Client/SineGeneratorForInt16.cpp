#include "SineGeneratorForInt16.h"
#include <QtMath>

SineGeneratorForInt16::SineGeneratorForInt16(qint32 bytes) {

    m_countOfBytes = bytes;
    m_lastSinePositionInSinusArray = 0;

    const qint16 size = sizeof(qint16);

    m_block.reserve(size * 1000);

    for (int i = 0; i < 1000; ++i) {
        double tempValue = qSin(i * 2 * M_PI / 1000) * double(INT16_MAX);
        qint16 sineValueForQByteArray = tempValue > double(INT16_MAX) ? INT16_MAX :
                                            tempValue < double(INT16_MIN) ? INT16_MIN : qint16(tempValue);
        m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
    }
}

SineGeneratorForInt16::~SineGeneratorForInt16() {}

QByteArray SineGeneratorForInt16::generateSineForType() {

    qint16 size = sizeof(qint16);
    QByteArray block;
    block.reserve(m_countOfBytes * size);

    int count = m_block.size() / size;

    if (m_countOfBytes + m_lastSinePositionInSinusArray < count)
        block.append(m_block.constData() + m_lastSinePositionInSinusArray * size,
                     m_countOfBytes * size);
    else {
        int partCountOfBytesForSendToServer = m_countOfBytes + m_lastSinePositionInSinusArray - count;
        block.append(m_block.constData() + m_lastSinePositionInSinusArray * size,
                     m_countOfBytes * size - partCountOfBytesForSendToServer * size);
        block.append(m_block.constData(), partCountOfBytesForSendToServer * size);
    }

    m_lastSinePositionInSinusArray += m_countOfBytes;

    if (m_lastSinePositionInSinusArray >= 1000)
        m_lastSinePositionInSinusArray -= 1000;

    return block;
}

