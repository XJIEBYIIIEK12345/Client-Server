#ifndef PackageDataArray_H
#define PackageDataArray_H

#include "Package.h"

class PackageDataArray : public Package
{
public:
  PackageDataArray();
  PackageDataArray(qint32 count, QVariantList data);
  PackageDataArray(const PackageDataArray& pack);
  PackageDataArray(const QVariantMap& map);
  ~PackageDataArray();

  void setPackageDataArrayData(qint32 count, QVariantList data);
  void setPackageDataArrayData(const PackageDataArray& pack);

  QMap<QString, QVariant> valuesToMap() const override;

public:
  QVariantList m_data;
};

#endif // PackageDataArray_H
