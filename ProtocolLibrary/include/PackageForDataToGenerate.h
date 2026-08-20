#ifndef PACKAGEFORDATATOGENERATE_H
#define PACKAGEFORDATATOGENERATE_H

#include "Package.h"

class PackageForDataToGenerate : public Package
{
public:
  PackageForDataToGenerate();
  PackageForDataToGenerate(qint32 count, MessageType type, QString valueType,
                           quint32 bytes);
  PackageForDataToGenerate(const PackageForDataToGenerate& pack);
  PackageForDataToGenerate(const QVariantMap& map);
  ~PackageForDataToGenerate();

  void setPackageForDataToGenerateData(qint32 count, MessageType type,
                                       QString valueType, quint32 bytes);
  void setPackageForDataToGenerateData(const PackageForDataToGenerate& pack);

  QMap<QString, QVariant> valuesToMap() const override;

public:
  QString m_valueType;
  quint32 m_bytes;
};

#endif // PACKAGEFORDATATOGENERATE_H
