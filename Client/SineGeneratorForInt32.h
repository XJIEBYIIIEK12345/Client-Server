#ifndef SINEGENERATORFORINT32_H
#define SINEGENERATORFORINT32_H
#include "SineGenerator.h"

class SineGeneratorForInt32 : public SineGenerator {

public:
    SineGeneratorForInt32(qint32 bytes);
    ~SineGeneratorForInt32();

    QByteArray generateSineForType();
};

#endif // SINEGENERATORFORINT32_H
