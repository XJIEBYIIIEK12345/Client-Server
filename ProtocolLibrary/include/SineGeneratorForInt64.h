#ifndef SINEGENERATORFORINT64_H
#define SINEGENERATORFORINT64_H
#include "SineGenerator.h"

class SineGeneratorForInt64 : public SineGenerator {

public:
    SineGeneratorForInt64();
    ~SineGeneratorForInt64();

    QByteArray generateSineForType(quint32 countOfBytes);
};

#endif // SINEGENERATORFORINT64_H
