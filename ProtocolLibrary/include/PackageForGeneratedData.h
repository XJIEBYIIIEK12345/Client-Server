#ifndef PACKAGEFORGENERATEDDATA_H
#define PACKAGEFORGENERATEDDATA_H

#include "Package.h"

class PackageForGeneratedData : public Package
{
public:
  PackageForGeneratedData();
  PackageForGeneratedData(qint32 count, MessageType type, QByteArray data);
  PackageForGeneratedData(const PackageForGeneratedData& pack);
  PackageForGeneratedData(const QVariantMap& map);
  ~PackageForGeneratedData();

  void setPackageForGeneratedDataData(qint32 count, MessageType type,
                                      QByteArray data);
  void setPackageForGeneratedDataData(const PackageForGeneratedData& pack);

  QMap<QString, QVariant> valuesToMap() const override;

public:
  QByteArray m_data;
};

#endif // PACKAGEFORGENERATEDDATA_H
