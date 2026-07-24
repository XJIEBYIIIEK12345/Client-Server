#include "SineGeneratorForInt64.h"
#include <QtMath>

SineGeneratorForInt64::SineGeneratorForInt64(qint32 bytes) {

    m_countOfBytes = bytes;
    m_lastSinePositionInSinusArray = 0;

    const qint16 size = sizeof(qint64);

    m_block.reserve(size * 1000);

    for (int i = 0; i < 1000; ++i) {
        double tempValue = qSin(i * 2 * M_PI / 1000) * double(std::numeric_limits<qint64>::max());
        qint64 sineValueForQByteArray = tempValue > double(std::numeric_limits<qint64>::max()) ? std::numeric_limits<qint64>::max() :
                                            tempValue < double(std::numeric_limits<qint64>::min()) ? std::numeric_limits<qint64>::min() : qint64(tempValue);

        m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
    }
}

SineGeneratorForInt64::~SineGeneratorForInt64() {}

QByteArray SineGeneratorForInt64::generateSineForType() {

    qint16 size = sizeof(qint64);
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

