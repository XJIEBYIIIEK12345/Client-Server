#ifndef SINEGENERATORFORINT16_H
#define SINEGENERATORFORINT16_H

#include "SineGenerator.h"

class SineGeneratorForInt16 : public SineGenerator
{
public:
  SineGeneratorForInt16();
  ~SineGeneratorForInt16();

  QByteArray generateSineForType(quint32 countOfBytes);
};

#endif // SINEGENERATORFORINT16_H
