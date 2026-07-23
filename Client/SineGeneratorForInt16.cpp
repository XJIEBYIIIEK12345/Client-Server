#include "SineGeneratorForInt16.h"
#include <QtMath>

SineGeneratorForInt16::SineGeneratorForInt16() {

    qint16 size = sizeof(qint16);

    m_block.reserve(size * 1000);

    qint16 sineValueForQByteArray = 0;

    for (int i = 0; i < 1000; ++i) {
        sineValueForQByteArray = qSin(i * 2 * M_PI / 1000) * INT16_MAX;
        m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
    }
}

SineGeneratorForInt16::~SineGeneratorForInt16() {}

QByteArray SineGeneratorForInt16::generateSineForType(int countOfBytesForSendToServer, int lastSinePositionInSinusArray) {

    qint16 size = sizeof(qint16);
    QByteArray block;
    block.reserve(countOfBytesForSendToServer * size);

    int count = m_block.size() / size;

    if (countOfBytesForSendToServer + lastSinePositionInSinusArray < count)
        block.append(m_block.constData() + lastSinePositionInSinusArray * size,
                     countOfBytesForSendToServer * size);
    else {
        int partCountOfBytesForSendToServer = countOfBytesForSendToServer + lastSinePositionInSinusArray - count;
        block.append(m_block.constData() + lastSinePositionInSinusArray * size,
                     countOfBytesForSendToServer * size - partCountOfBytesForSendToServer * size);
        block.append(m_block.constData(), partCountOfBytesForSendToServer * size);
    }

    return block;
}

