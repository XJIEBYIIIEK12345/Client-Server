#include "SineGeneratorForInt32.h"
#include <QtMath>

SineGeneratorForInt32::SineGeneratorForInt32()
{
  m_lastSinePositionInSinusArray = 0;

  const qint16 size = sizeof(qint32);

  m_block.reserve(size * 1000);

  for (int i = 0; i < 1000; ++i)
  {
    double tempValue =
        qSin(i * 2 * M_PI / 1000) * double(std::numeric_limits<qint32>::max());
    qint32 sineValueForQByteArray =
        tempValue > double(std::numeric_limits<qint32>::max())
            ? std::numeric_limits<qint32>::max()
        : tempValue < double(std::numeric_limits<qint32>::min())
            ? std::numeric_limits<qint32>::min()
            : qint32(tempValue);
    m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
  }
}

SineGeneratorForInt32::~SineGeneratorForInt32() {}

QByteArray SineGeneratorForInt32::generateSineForType(quint32 countOfBytes)
{
  qint16 size = sizeof(qint32);

  if (m_lastSinePositionInSinusArray >= quint32(m_block.size() / size))
  {
    m_lastSinePositionInSinusArray = 0;
  }

  QByteArray block;
  block.reserve(countOfBytes * size);

  quint32 tempBytes = 0;
  while (tempBytes < countOfBytes)
  {

    quint32 bytesLeft = countOfBytes - tempBytes;
    quint32 bytesAvailable = m_block.size() / size - m_lastSinePositionInSinusArray;
    quint32 chunk = qMin(bytesLeft, bytesAvailable);

    block.append(m_block.constData() + m_lastSinePositionInSinusArray * size,
                 chunk * size);

    tempBytes += chunk;
    m_lastSinePositionInSinusArray =
        (m_lastSinePositionInSinusArray + chunk) % (m_block.size() / size);
  }

  return block;
}
