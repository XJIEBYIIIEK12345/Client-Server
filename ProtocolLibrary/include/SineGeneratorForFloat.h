#ifndef SINEGENERATORFORFLOAT_H
#define SINEGENERATORFORFLOAT_H

#include "SineGenerator.h"

class SineGeneratorForFloat : public SineGenerator
{
public:
  SineGeneratorForFloat();
  ~SineGeneratorForFloat();

  QByteArray generateSineForType(quint32 countOfBytes);
};

#endif // SINEGENERATORFORFLOAT_H
