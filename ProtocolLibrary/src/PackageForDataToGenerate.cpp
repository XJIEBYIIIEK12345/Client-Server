#include "PackageForDataToGenerate.h"
#include "Package.h"

PackageForDataToGenerate::PackageForDataToGenerate()
  : Package()
{}

PackageForDataToGenerate::PackageForDataToGenerate(qint32 id, MessageType type,
                                                   QString valueType, quint32 bytes)
{
  m_id = id;
  m_type = type;
  m_valueType = valueType;
  m_bytes = bytes;
}

PackageForDataToGenerate::PackageForDataToGenerate(
    const PackageForDataToGenerate& pack)
  : Package(pack)
{
  // m_id = pack.m_id;
  // m_type = pack.m_type;
  m_valueType = pack.m_valueType;
  m_bytes = pack.m_bytes;
}

PackageForDataToGenerate::PackageForDataToGenerate(const QVariantMap& map)
{
  m_id = map["id"].toInt();
  m_type = MessageType(map["type"].toInt());
  m_valueType = map["valueType"].toString();
  m_bytes = map["bytes"].toInt();
}

PackageForDataToGenerate::~PackageForDataToGenerate() {}

void PackageForDataToGenerate::setPackageForDataToGenerateData(qint32 id,
                                                               MessageType type,
                                                               QString valueType,
                                                               quint32 bytes)
{
  m_id = id;
  m_type = type;
  m_valueType = valueType;
  m_bytes = bytes;
}

void PackageForDataToGenerate::setPackageForDataToGenerateData(
    const PackageForDataToGenerate& pack)
{
  m_id = pack.m_id;
  m_type = pack.m_type;
  m_valueType = pack.m_valueType;
  m_bytes = pack.m_bytes;
}

QMap<QString, QVariant> PackageForDataToGenerate::valuesToMap() const
{
  QMap<QString, QVariant> map = Package::valuesToMap();

  map.insert("valueType", m_valueType);
  map.insert("bytes", m_bytes);

  return map;
}
