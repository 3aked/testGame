#include "errors.h"

ErrorData::ErrorData(EError error, const QString &desc)
    : m_error{error}, m_desc{desc} {}

ErrorData::ErrorData(const QJsonObject &json) {
  if (!json.contains("error")) {
    throw ErrorData(EError::BadData, "error missing");
  } else {
    auto value = json.value("error");
    if (value.isDouble()) {
      m_error = (EError)value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid error");
    }
  }

  m_desc = json.value("errorDesc").toString("");
}

QString ErrorData::errorString() const {
  QString errStr;
  switch (m_error) {
  case EError::BadData:
    errStr = "BadDataRequest";
    break;
  case EError::NetworkError:
    errStr = "NetworkError";
    break;
  default:
    assert(false);
  }
  if (!m_desc.isEmpty()) {
    errStr += ": " + m_desc;
  }
  return errStr;
}

QJsonObject ErrorData::toJson() const {
  QJsonObject obj;
  obj.insert("error", (int)m_error);
  obj.insert("errorDesc", m_desc);
  return obj;
}
