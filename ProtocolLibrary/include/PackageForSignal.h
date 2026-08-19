#ifndef PACKAGEFORSIGNAL_H
#define PACKAGEFORSIGNAL_H

#include "Package.h"

class PackageForSignal : public Package
{
public:
  PackageForSignal();
  PackageForSignal(qint32 count, MessageType type, bool flag);
  PackageForSignal(const PackageForSignal& pack);
  PackageForSignal(const QVariantMap& map);
  ~PackageForSignal();

  void setPackageForSignalData(qint32 count, MessageType type, bool flag);
  void setPackageForSignalData(const PackageForSignal& pack);

  QMap<QString, QVariant> valuesToMap() const override;

public:
  bool m_flag;
};

#endif // PACKAGEFORSIGNAL_H
