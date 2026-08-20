#include "SineGeneratorForInt64.h"
#include <QtMath>

SineGeneratorForInt64::SineGeneratorForInt64()
{
  m_lastSinePositionInSinusArray = 0;

  const qint16 size = sizeof(qint64);

  m_block.reserve(size * 1000);

  for (int i = 0; i < 1000; ++i)
  {
    double tempValue =
        qSin(i * 2 * M_PI / 1000) * double(std::numeric_limits<qint64>::max());
    qint64 sineValueForQByteArray =
        tempValue > double(std::numeric_limits<qint64>::max())
            ? std::numeric_limits<qint64>::max()
        : tempValue < double(std::numeric_limits<qint64>::min())
            ? std::numeric_limits<qint64>::min()
            : qint64(tempValue);

    m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
  }
}

SineGeneratorForInt64::SineGeneratorForInt64(quint32 countOfBytes)
  : SineGeneratorForInt64()
{
  m_countOfBytes = countOfBytes;
}

SineGeneratorForInt64::~SineGeneratorForInt64() {}

void SineGeneratorForInt64::setCountOfBytes(quint32 countOfBytes)
{
  m_countOfBytes = countOfBytes;
}

QByteArray SineGeneratorForInt64::generateSineForType()
{
  qint16 size = sizeof(qint64);

  if (m_lastSinePositionInSinusArray >= quint32(m_block.size() / size))
  {
    m_lastSinePositionInSinusArray = 0;
  }

  QByteArray block;
  block.reserve(m_countOfBytes * size);

  quint32 tempBytes = 0;
  while (tempBytes < m_countOfBytes)
  {
    quint32 bytesLeft = m_countOfBytes - tempBytes;
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
