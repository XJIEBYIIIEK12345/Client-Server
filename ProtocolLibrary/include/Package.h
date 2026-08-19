#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>
#include <QVariant>

enum MessageType
{
  MetaDataRequest = 0,
  MetaDataResponse,
  SinAnswer,
  SinConfirmation,
  Count
};

class Package
{
protected:
  Package();
  Package(qint32 id, MessageType type);
  Package(const Package& pack);

public:
  virtual ~Package();
  virtual void setPackageData(qint32 id, MessageType type);
  virtual void setPackageData(const Package& pack);

  virtual QMap<QString, QVariant> valuesToMap() const;

public:
  qint32 m_id;
  MessageType m_type;
};

#endif // PACKAGE_H
