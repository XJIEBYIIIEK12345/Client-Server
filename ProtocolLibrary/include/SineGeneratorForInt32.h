#ifndef SINEGENERATORFORINT32_H
#define SINEGENERATORFORINT32_H

#include "SineGenerator.h"

class SineGeneratorForInt32 : public SineGenerator
{
public:
  SineGeneratorForInt32();
  ~SineGeneratorForInt32();

  QByteArray generateSineForType(quint32 countOfBytes);
};

#endif // SINEGENERATORFORINT32_H
