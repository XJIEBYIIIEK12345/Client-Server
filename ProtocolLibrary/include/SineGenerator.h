#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H
#include <QByteArray>
#include <QString>

enum class SineGeneratorType {

    SineGeneratorForInt16,
    SineGeneratorForInt32,
    SineGeneratorForInt64,
    SineGeneratorForFloat,
    SineGeneratorForDouble
};

class SineGenerator {

public:
    virtual ~SineGenerator() = default;
    virtual QByteArray generateSineForType() = 0;

    static SineGenerator* makeGenerator(QString str, quint64 bytes);

protected:
    QByteArray m_block;
    qint32 m_lastSinePositionInSinusArray = 0;
    qint32 m_countOfBytes = 0;
};

#endif // SINEGENERATOR_H
