#ifndef SINEGENERATORFORFLOAT_H
#define SINEGENERATORFORFLOAT_H

#include "SineGenerator.h"

class SineGeneratorForFloat : public SineGenerator
{
public:
  SineGeneratorForFloat();
  SineGeneratorForFloat(quint32 countOfBytes);
  ~SineGeneratorForFloat();

  void setCountOfBytes(quint32 countOfBytes) override;
  QByteArray generateSineForType() override;
};

#endif // SINEGENERATORFORFLOAT_H
