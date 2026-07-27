#include "SineGenerator.h"
#include "SineGeneratorForInt16.h"
#include "SineGeneratorForInt32.h"
#include "SineGeneratorForInt64.h"
#include "SineGeneratorForFloat.h"
#include "SineGeneratorForDouble.h"

SineGenerator *SineGenerator::makeGenerator(QString str, quint64 bytes) {

    if (str == "qint16") {
        return new SineGeneratorForInt16(bytes);
    }
    else if (str == "qint32") {
        return new SineGeneratorForInt32(bytes);
    }
    else if (str == "qint64") {
        return new SineGeneratorForInt64(bytes);
    }
    else if (str == "float") {
        return new SineGeneratorForFloat(bytes);
    }
    else if (str == "double") {
        return new SineGeneratorForDouble(bytes);
    }
    else {
        return new SineGeneratorForInt32(bytes);
    }
}
