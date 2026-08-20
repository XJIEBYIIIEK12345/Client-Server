#include "SineGeneratorForInt16.h"
#include <QtMath>

SineGeneratorForInt16::SineGeneratorForInt16()
{
  m_lastSinePositionInSinusArray = 0;

  const qint16 size = sizeof(qint16);

  m_block.reserve(size * 1000);

  for (int i = 0; i < 1000; ++i)
  {
    double tempValue =
        qSin(i * 2 * M_PI / 1000) * double(std::numeric_limits<qint16>::max());
    qint16 sineValueForQByteArray =
        tempValue > double(std::numeric_limits<qint16>::max())
            ? std::numeric_limits<qint16>::max()
        : tempValue < double(std::numeric_limits<qint16>::min())
            ? std::numeric_limits<qint16>::min()
            : qint16(tempValue);
    m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
  }
}

SineGeneratorForInt16::SineGeneratorForInt16(quint32 countOfBytes)
  : SineGeneratorForInt16()
{
  m_countOfBytes = countOfBytes;
}

SineGeneratorForInt16::~SineGeneratorForInt16() {}

void SineGeneratorForInt16::setCountOfBytes(quint32 countOfBytes)
{
  m_countOfBytes = countOfBytes;
}

QByteArray SineGeneratorForInt16::generateSineForType()
{
  qint16 size = sizeof(qint16);

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
