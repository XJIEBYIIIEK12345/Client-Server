#ifndef PACKAGEMETADATA_H
#define PACKAGEMETADATA_H

#include "Package.h"

class PackageMetaData : public Package
{
public:
  PackageMetaData();
  PackageMetaData(qint32 count, QString valueType, quint32 bytes);
  PackageMetaData(const PackageMetaData& pack);
  PackageMetaData(const QVariantMap& map);
  ~PackageMetaData();

  void setPackageMetaDataData(qint32 count, QString valueType, quint32 bytes);
  void setPackageMetaDataData(const PackageMetaData& pack);

  QMap<QString, QVariant> valuesToMap() const override;

public:
  QString m_valueType;
  quint32 m_bytes;
};

#endif // PACKAGEMETADATA_H
