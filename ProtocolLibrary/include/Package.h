#ifndef PACKAGE_H
#define PACKAGE_H
#include <QString>
class Package {
public:
    Package();
    Package(qint32 count, QString type, QByteArray data);
    Package(const Package& pack);
    ~Package();

    void setPackageData(qint32 count, QString type, QByteArray data);
    void setPackageData(const Package& pack);

public:
    qint32 m_count;
    QString m_type;
    QByteArray m_data;
};

#endif // PACKAGE_H
