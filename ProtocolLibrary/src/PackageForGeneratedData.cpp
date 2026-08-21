#include "Package.h"
#include "PackageForGeneratedData.h"

PackageForGeneratedData::PackageForGeneratedData()
  : Package()
{}

PackageForGeneratedData::PackageForGeneratedData(qint32 id, MessageType type,
                                                 QByteArray data)
  : Package(id, type)
{
  m_data = data;
}

PackageForGeneratedData::PackageForGeneratedData(const PackageForGeneratedData& pack)
  : Package(pack)
{
  m_data = pack.m_data;
}

PackageForGeneratedData::PackageForGeneratedData(const QVariantMap& map)
{
  m_id = map["id"].toInt();
  m_type = MessageType(map["type"].toInt());
  m_data = QByteArray::fromBase64(map["data"].toByteArray());
}

PackageForGeneratedData::~PackageForGeneratedData() {}

void PackageForGeneratedData::setPackageForGeneratedDataData(qint32 id,
                                                             MessageType type,
                                                             QByteArray data)
{
  m_id = id;
  m_type = type;
  m_data = data;
}

void PackageForGeneratedData::setPackageForGeneratedDataData(
    const PackageForGeneratedData& pack)
{
  m_id = pack.m_id;
  m_type = pack.m_type;
  m_data = pack.m_data;
}

QMap<QString, QVariant> PackageForGeneratedData::valuesToMap() const
{
  QMap<QString, QVariant> map = Package::valuesToMap();

  map.insert("data", m_data.toBase64());

  return map;
}
