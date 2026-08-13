#include "Package.h"

Package::Package()
  : m_count(0)
{}

Package::Package(qint32 count, MessageType type, QByteArray data)
{
  m_count = count;
  m_type = type;
  m_data = data;
}

Package::Package(const Package& package)
{
  m_count = package.m_count;
  m_type = package.m_type;
  m_data = package.m_data;
}

Package::~Package() {}

void Package::setPackageData(qint32 count, MessageType type, QByteArray data)
{
  m_count = count;
  m_type = type;
  m_data = data;
}

void Package::setPackageData(const Package& package)
{
  m_count = package.m_count;
  m_type = package.m_type;
  m_data = package.m_data;
}
