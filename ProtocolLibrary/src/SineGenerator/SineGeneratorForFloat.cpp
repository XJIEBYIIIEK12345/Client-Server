#include "SineGeneratorForFloat.h"
#include <QtMath>

SineGeneratorForFloat::SineGeneratorForFloat()
{
  m_arr = new float[1000];
  m_lastSinePositionInSinusArray = 0;

  for (int i = 0; i < 1000; ++i)
  {
    m_arr[i] = qSin(i * 2 * M_PI / 1000);
  }
}

SineGeneratorForFloat::SineGeneratorForFloat(quint32 countOfBytes)
  : SineGeneratorForFloat()
{
  m_countOfBytes = countOfBytes;
}

SineGeneratorForFloat::~SineGeneratorForFloat()
{
  if (m_arr != nullptr)
  {
    delete m_arr;
    m_arr = nullptr;
  }
}

void SineGeneratorForFloat::setCountOfBytes(quint32 countOfBytes)
{
  m_countOfBytes = countOfBytes;
}

QVariantList SineGeneratorForFloat::generateSineForType()
{
  if (m_lastSinePositionInSinusArray >= 1000)
  {
    m_lastSinePositionInSinusArray = 0;
  }

  QVector<float> genVector;
  genVector.reserve(m_countOfBytes);

  quint32 tempBytes = 0;
  while (tempBytes < m_countOfBytes)
  {
    quint32 bytesLeft = m_countOfBytes - tempBytes;
    quint32 bytesAvailable = 1000 - m_lastSinePositionInSinusArray;
    quint32 chunk = qMin(bytesLeft, bytesAvailable);

    std::copy_n(m_arr + m_lastSinePositionInSinusArray, chunk,
                std::back_inserter(genVector));

    tempBytes += chunk;
    m_lastSinePositionInSinusArray = (m_lastSinePositionInSinusArray + chunk) % 1000;
  }

  QList<float> genList = genVector.toList();
  QVariantList genVariantList;
  genVariantList.reserve(genList.size());

  std::copy(genList.begin(), genList.end(), std::back_inserter(genVariantList));

  return genVariantList;
}
