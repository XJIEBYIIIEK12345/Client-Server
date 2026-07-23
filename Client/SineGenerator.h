#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H
#include <QByteArray>
#include <QString>

class SineGenerator {

public:
    virtual ~SineGenerator() = default;
    virtual QByteArray generateSineForType() = 0;

protected:
    QByteArray m_block;
    qint32 m_lastSinePositionInSinusArray = 0;
    qint32 m_countOfBytes = 0;
};

#endif // SINEGENERATOR_H
