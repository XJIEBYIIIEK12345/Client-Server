#include "Package.h"
#include "PackageMetaData.h"

PackageMetaData::PackageMetaData()
  : Package()
{}

PackageMetaData::PackageMetaData(qint32 id, QString valueType, quint32 bytes)
  : Package(id, MessageType::MetaDataResponse)
{
  m_valueType = valueType;
  m_bytes = bytes;
}

PackageMetaData::PackageMetaData(const PackageMetaData& pack)
  : Package(pack)
{
  m_valueType = pack.m_valueType;
  m_bytes = pack.m_bytes;
}

PackageMetaData::PackageMetaData(const QVariantMap& map)
{
  m_id = map["id"].toInt();
  m_type = MessageType(map["type"].toInt());
  m_valueType = map["valueType"].toString();
  m_bytes = map["bytes"].toInt();
}

PackageMetaData::~PackageMetaData() {}

void PackageMetaData::setPackageMetaDataData(qint32 id, QString valueType,
                                             quint32 bytes)
{
  m_id = id;
  m_type = MessageType::MetaDataResponse;
  m_valueType = valueType;
  m_bytes = bytes;
}

void PackageMetaData::setPackageMetaDataData(const PackageMetaData& pack)
{
  m_id = pack.m_id;
  m_type = pack.m_type;
  m_valueType = pack.m_valueType;
  m_bytes = pack.m_bytes;
}

QMap<QString, QVariant> PackageMetaData::valuesToMap() const
{
  QMap<QString, QVariant> map = Package::valuesToMap();

  map.insert("valueType", m_valueType);
  map.insert("bytes", m_bytes);

  return map;
}
