#include "SineGeneratorForInt64.h"
#include <QtMath>

SineGeneratorForInt64::SineGeneratorForInt64()
{
  m_arr = new qint64[1000];
  m_lastSinePositionInSinusArray = 0;

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
    m_arr[i] = sineValueForQByteArray;
  }
}

SineGeneratorForInt64::SineGeneratorForInt64(quint32 countOfBytes)
  : SineGeneratorForInt64()
{
  m_countOfBytes = countOfBytes;
}

SineGeneratorForInt64::~SineGeneratorForInt64()
{
  if (m_arr != nullptr)
  {
    delete m_arr;
    m_arr = nullptr;
  }
}

void SineGeneratorForInt64::setCountOfBytes(quint32 countOfBytes)
{
  m_countOfBytes = countOfBytes;
}

QVariantList SineGeneratorForInt64::generateSineForType()
{
  if (m_lastSinePositionInSinusArray >= 1000)
  {
    m_lastSinePositionInSinusArray = 0;
  }

  QVector<qint64> genVector;
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

  QList<qint64> genList = genVector.toList();
  QVariantList genVariantList;
  genVariantList.reserve(genList.size());

  std::copy(genList.begin(), genList.end(), std::back_inserter(genVariantList));

  return genVariantList;
}
