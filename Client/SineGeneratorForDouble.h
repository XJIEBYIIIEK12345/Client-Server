#ifndef SINEGENERATORFORDOUBLE_H
#define SINEGENERATORFORDOUBLE_H
#include "SineGenerator.h"

class SineGeneratorForDouble : public SineGenerator {

public:
    SineGeneratorForDouble(qint32 bytes);
    ~SineGeneratorForDouble();

    QByteArray generateSineForType();
};

#endif // SINEGENERATORFORDOUBLE_H
