#include "Package.h"
#include "PackageDataArray.h"

PackageDataArray::PackageDataArray()
  : Package()
{}

PackageDataArray::PackageDataArray(qint32 id, QVariantList data)
  : Package(id, MessageType::SinAnswer)
{
  m_data = data;
}

PackageDataArray::PackageDataArray(const PackageDataArray& pack)
  : Package(pack)
{
  m_data = pack.m_data;
}

PackageDataArray::PackageDataArray(const QVariantMap& map)
{
  m_id = map["id"].toInt();
  m_type = MessageType(map["type"].toInt());
  m_data = map["data"].toList();
}

PackageDataArray::~PackageDataArray() {}

void PackageDataArray::setPackageDataArrayData(qint32 id, QVariantList data)
{
  m_id = id;
  m_type = MessageType::SinAnswer;
  m_data = data;
}

void PackageDataArray::setPackageDataArrayData(const PackageDataArray& pack)
{
  m_id = pack.m_id;
  m_type = pack.m_type;
  m_data = pack.m_data;
}

QMap<QString, QVariant> PackageDataArray::valuesToMap() const
{
  QMap<QString, QVariant> map = Package::valuesToMap();

  map.insert("data", m_data);

  return map;
}
