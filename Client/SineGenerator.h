#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H
#include <QByteArray>

class SineGenerator {

public:
    virtual ~SineGenerator() = default;
    virtual QByteArray generateSineForType(int countOfBytesForSendToServer = 0, int lastSinePositionInSinusArray = 0) = 0;

protected:
    QByteArray m_block;
};

#endif // SINEGENERATOR_H
