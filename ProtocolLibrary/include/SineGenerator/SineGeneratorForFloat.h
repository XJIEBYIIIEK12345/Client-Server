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
  QVariantList generateSineForType() override;

private:
  float* m_arr;
};

#endif // SINEGENERATORFORFLOAT_H
