#include "SineGeneratorForDouble.h"
#include <QDebug>
#include <QtMath>

SineGeneratorForDouble::SineGeneratorForDouble()
{
  m_lastSinePositionInSinusArray = 0;

  const qint16 size = sizeof(double);

  m_block.reserve(size * 1000);

  for (int i = 0; i < 1000; ++i)
  {
    double sineValueForQByteArray = qSin(i * 2 * M_PI / 1000);
    m_block.append(reinterpret_cast<char*>(&sineValueForQByteArray), size);
  }
}

SineGeneratorForDouble::~SineGeneratorForDouble() {}

QByteArray SineGeneratorForDouble::generateSineForType(quint32 countOfBytes)
{
  qint16 size = sizeof(double);

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
