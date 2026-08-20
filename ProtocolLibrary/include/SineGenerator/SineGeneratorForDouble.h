#ifndef SINEGENERATORFORDOUBLE_H
#define SINEGENERATORFORDOUBLE_H

#include "SineGenerator.h"

class SineGeneratorForDouble : public SineGenerator
{
public:
  SineGeneratorForDouble();
  SineGeneratorForDouble(quint32 countOfBytes);
  ~SineGeneratorForDouble();

  void setCountOfBytes(quint32 countOfBytes) override;
  QByteArray generateSineForType() override;
};

#endif // SINEGENERATORFORDOUBLE_H
