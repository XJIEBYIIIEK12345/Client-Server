#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H

#include <QByteArray>
#include <QString>

enum class SineGeneratorType
{
  SineGeneratorForInt16,
  SineGeneratorForInt32,
  SineGeneratorForInt64,
  SineGeneratorForFloat,
  SineGeneratorForDouble
};

class SineGenerator
{
public:
  virtual ~SineGenerator() = default;
  virtual QByteArray generateSineForType() = 0;
  virtual void setCountOfBytes(quint32 countOfBytes) = 0;

  static SineGenerator* makeGenerator(QString str);

protected:
  QByteArray m_block;
  quint32 m_lastSinePositionInSinusArray;
  quint32 m_countOfBytes;
};

#endif // SINEGENERATOR_H
