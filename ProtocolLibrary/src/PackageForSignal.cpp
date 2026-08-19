#include "PackageForSignal.h"
#include "Package.h"

PackageForSignal::PackageForSignal()
  : Package()
{}

PackageForSignal::PackageForSignal(qint32 id, MessageType type, bool flag)
{
  m_id = id;
  m_type = type;
  m_flag = flag;
}

PackageForSignal::PackageForSignal(const PackageForSignal& pack)
  : Package(pack)
{
  // m_id = pack.m_id;
  // m_type = pack.m_type;
  m_flag = pack.m_flag;
}

PackageForSignal::PackageForSignal(const QVariantMap& map)
{
  m_id = map["id"].toInt();
  m_type = MessageType(map["type"].toInt());
  m_flag = map["flag"].toBool();
}

PackageForSignal::~PackageForSignal() {}

void PackageForSignal::setPackageForSignalData(qint32 id, MessageType type,
                                               bool flag)
{
  m_id = id;
  m_type = type;
  m_flag = flag;
}

void PackageForSignal::setPackageForSignalData(const PackageForSignal& pack)
{
  m_id = pack.m_id;
  m_type = pack.m_type;
  m_flag = pack.m_flag;
}

QMap<QString, QVariant> PackageForSignal::valuesToMap() const
{
  QMap<QString, QVariant> map = Package::valuesToMap();

  map.insert("flag", m_flag);

  return map;
}
