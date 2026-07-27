#ifndef SINEGENERATORFORINT64_H
#define SINEGENERATORFORINT64_H
#include "SineGenerator.h"

class SineGeneratorForInt64 : public SineGenerator {

public:
    SineGeneratorForInt64(qint32 bytes);
    ~SineGeneratorForInt64();

    QByteArray generateSineForType();
};

#endif // SINEGENERATORFORINT64_H
