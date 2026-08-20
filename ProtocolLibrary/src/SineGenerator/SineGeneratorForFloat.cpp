#include "SineGeneratorForFloat.h"
#include <QtMath>

SineGeneratorForFloat::SineGeneratorForFloat()
{
  m_lastSinePositionInSinusArray = 0;

  const qint16 size = sizeof(float);

  m_block.reserve(size * 1000);

  for (int i = 0; i < 1000; ++i)
  {
    float sineValueForQByteArray = qSin(i * 2 * M_PI / 1000);
    m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
  }
}

SineGeneratorForFloat::SineGeneratorForFloat(quint32 countOfBytes)
  : SineGeneratorForFloat()
{
  m_countOfBytes = countOfBytes;
}

SineGeneratorForFloat::~SineGeneratorForFloat() {}

void SineGeneratorForFloat::setCountOfBytes(quint32 countOfBytes)
{
  m_countOfBytes = countOfBytes;
}

QByteArray SineGeneratorForFloat::generateSineForType()
{
  qint16 size = sizeof(float);

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
