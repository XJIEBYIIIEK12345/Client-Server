#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H

#include <QVariant>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

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
  virtual QVariantList generateSineForType() = 0;
  virtual void setCountOfBytes(quint32 countOfBytes) = 0;

  static SineGenerator* makeGenerator(QString str);

protected:
  quint32 m_lastSinePositionInSinusArray;
  quint32 m_countOfBytes;
};

#endif // SINEGENERATOR_H
