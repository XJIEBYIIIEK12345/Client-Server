#ifndef SINEGENERATORFORINT64_H
#define SINEGENERATORFORINT64_H

#include "SineGenerator.h"

class SineGeneratorForInt64 : public SineGenerator
{
public:
  SineGeneratorForInt64();
  SineGeneratorForInt64(quint32 countOfBytes);
  ~SineGeneratorForInt64();

  void setCountOfBytes(quint32 countOfBytes) override;
  QVariantList generateSineForType() override;

private:
  qint64* m_arr;
};

#endif // SINEGENERATORFORINT64_H
