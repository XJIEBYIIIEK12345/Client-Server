#ifndef SINEGENERATORFORDOUBLE_H
#define SINEGENERATORFORDOUBLE_H
#include "SineGenerator.h"

class SineGeneratorForDouble : public SineGenerator {

public:
    SineGeneratorForDouble();
    ~SineGeneratorForDouble();

    QByteArray generateSineForType(quint32 countOfBytes);
};

#endif // SINEGENERATORFORDOUBLE_H
