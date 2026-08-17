#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>

enum MessageType
{
  BytesAndTypeRequest = 0,
  SinRequest,
  SinAnswer,
  SinConfirmation,
  Count
};

class Package
{
public:
  Package();
  Package(qint32 count, MessageType type, QByteArray data);
  Package(const Package& pack);
  ~Package();

  void setPackageData(qint32 count, MessageType type, QByteArray data);
  void setPackageData(const Package& pack);

public:
  qint32 m_count;
  MessageType m_type;
  QByteArray m_data;
};

#endif // PACKAGE_H
