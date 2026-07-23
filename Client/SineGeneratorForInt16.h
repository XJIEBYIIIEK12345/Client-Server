#ifndef SINEGENERATORFORINT16_H
#define SINEGENERATORFORINT16_H
#include "SineGenerator.h"

class SineGeneratorForInt16 : public SineGenerator {

public:
    SineGeneratorForInt16(qint32 bytes);
    ~SineGeneratorForInt16();

    QByteArray generateSineForType();
};

#endif // SINEGENERATORFORINT16_H
