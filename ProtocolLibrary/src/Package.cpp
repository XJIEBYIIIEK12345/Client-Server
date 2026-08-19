#include "Package.h"

Package::Package()
  : m_id(0)
{}

Package::Package(qint32 id, MessageType type)
{
  m_id = id;
  m_type = type;
}

Package::Package(const Package& pack)
{
  m_id = pack.m_id;
  m_type = pack.m_type;
}

Package::~Package() {}

void Package::setPackageData(qint32 id, MessageType type)
{
  m_id = id;
  m_type = type;
}

void Package::setPackageData(const Package& pack)
{
  m_id = pack.m_id;
  m_type = pack.m_type;
}

QMap<QString, QVariant> Package::valuesToMap() const
{
  QMap<QString, QVariant> map;

  map.insert("id", m_id);
  map.insert("type", m_type);

  return map;
}
