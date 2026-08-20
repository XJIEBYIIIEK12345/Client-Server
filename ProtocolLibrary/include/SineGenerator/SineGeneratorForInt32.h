#ifndef SINEGENERATORFORINT32_H
#define SINEGENERATORFORINT32_H

#include "SineGenerator.h"

class SineGeneratorForInt32 : public SineGenerator
{
public:
  SineGeneratorForInt32();
  SineGeneratorForInt32(quint32 countOfBytes);
  ~SineGeneratorForInt32();

  void setCountOfBytes(quint32 countOfBytes) override;
  QByteArray generateSineForType() override;
};

#endif // SINEGENERATORFORINT32_H
