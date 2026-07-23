#ifndef SINEGENERATORFORINT16_H
#define SINEGENERATORFORINT16_H
#include "SineGenerator.h"

class SineGeneratorForInt16 : public SineGenerator {

public:
    SineGeneratorForInt16();
    ~SineGeneratorForInt16();

    QByteArray generateSineForType(int countOfBytesForSendToServer = 0, int lastSinePositionInSinusArray = 0);
};

#endif // SINEGENERATORFORINT16_H
