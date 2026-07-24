#ifndef SINEGENERATORFORFLOAT_H
#define SINEGENERATORFORFLOAT_H
#include "SineGenerator.h"

class SineGeneratorForFloat : public SineGenerator {

public:
    SineGeneratorForFloat(qint32 bytes);
    ~SineGeneratorForFloat();

    QByteArray generateSineForType();
};

#endif // SINEGENERATORFORFLOAT_H
