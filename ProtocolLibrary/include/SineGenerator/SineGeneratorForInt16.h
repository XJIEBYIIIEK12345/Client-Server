#ifndef SINEGENERATORFORINT16_H
#define SINEGENERATORFORINT16_H

#include "SineGenerator.h"

class SineGeneratorForInt16 : public SineGenerator
{
public:
  SineGeneratorForInt16();
  SineGeneratorForInt16(quint32 countOfBytes);
  ~SineGeneratorForInt16();

  void setCountOfBytes(quint32 countOfBytes) override;
  QVariantList generateSineForType() override;

private:
  qint16* m_arr;
};

#endif // SINEGENERATORFORINT16_H
