#ifndef SINEGENERATORFORINT64_H
#define SINEGENERATORFORINT64_H
#include "SineGenerator.h"

class SineGeneratorForInt64 : public SineGenerator {

public:
    SineGeneratorForInt64();
    ~SineGeneratorForInt64();

    QByteArray generateSineForType(int countOfBytesForSendToServer = 0, int lastSinePositionInSinusArray = 0);
};

#endif // SINEGENERATORFORINT64_H
