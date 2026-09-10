#include "SineGeneratorForDouble.h"
#include <QList>
#include <QVariant>
#include <QtMath>

SineGeneratorForDouble::SineGeneratorForDouble()
{
  m_arr = new double[1000];
  m_lastSinePositionInSinusArray = 0;

  for (int i = 0; i < 1000; ++i)
  {
    m_arr[i] = qSin(i * 2 * M_PI / 1000);
  }
}

SineGeneratorForDouble::SineGeneratorForDouble(quint32 countOfBytes)
  : SineGeneratorForDouble()
{
  m_countOfBytes = countOfBytes;
}

SineGeneratorForDouble::~SineGeneratorForDouble()
{
  if (m_arr != nullptr)
  {
    delete m_arr;
    m_arr = nullptr;
  }
}

void SineGeneratorForDouble::setCountOfBytes(quint32 countOfBytes)
{
  m_countOfBytes = countOfBytes;
}

QVariantList SineGeneratorForDouble::generateSineForType()
{
  if (m_lastSinePositionInSinusArray >= 1000)
  {
    m_lastSinePositionInSinusArray = 0;
  }

  QVector<double> genVector;
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

  QList<double> genList = genVector.toList();
  QVariantList genVariantList;
  genVariantList.reserve(genList.size());

  std::copy(genList.begin(), genList.end(), std::back_inserter(genVariantList));

  return genVariantList;
}
