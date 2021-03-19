#ifndef ERRORS_H
#define ERRORS_H

#include "data.h"

#include <QString>

enum class EError { BadData = 0, NetworkError };

class ErrorData : public Data {
public:
  ErrorData(EError error, const QString &desc = "");
  ErrorData(const QJsonObject &json);

  QString errorString() const;

  EError m_error;

  QString m_desc;

  // Data interface
  QJsonObject toJson() const override;
  EDataType type() const override { return Error; }
};

#endif // ERRORS_H
