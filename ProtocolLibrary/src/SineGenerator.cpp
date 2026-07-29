#include "SineGenerator.h"
#include "SineGeneratorForInt16.h"
#include "SineGeneratorForInt32.h"
#include "SineGeneratorForInt64.h"
#include "SineGeneratorForFloat.h"
#include "SineGeneratorForDouble.h"

SineGenerator *SineGenerator::makeGenerator(QString str) {

    if (str == "qint16") {
        return new SineGeneratorForInt16();
    }
    else if (str == "qint32") {
        return new SineGeneratorForInt32();
    }
    else if (str == "qint64") {
        return new SineGeneratorForInt64();
    }
    else if (str == "float") {
        return new SineGeneratorForFloat();
    }
    else if (str == "double") {
        return new SineGeneratorForDouble();
    }
    else {
        return new SineGeneratorForInt32();
    }
}
