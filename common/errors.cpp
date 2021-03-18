#include "errors.h"

Error::Error(EError error, const QString& desc)
    : m_error{error}, m_desc{desc} {}

Error::Error(const QJsonObject& json) {
    if (!json.contains("error")) {
        throw Error(EError::BadDataRequest, "error missing");
    } else {
        auto value = json.value("error");
        if (value.isDouble()) {
            m_error = (EError)value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid error");
        }
    }

    m_desc = json.value("errorDesc").toString("");
}

QString Error::errorString() const {
    QString errStr;
    switch (m_error) {
    case EError::BadDataRequest:
        errStr = "BadDataRequest";
        break;
    default:
         assert(false);
    }
    if (!m_desc.isEmpty()) {
        errStr += ": " + m_desc;
    }
    return errStr;
}

const char* Error::what() const noexcept {
    return errorString().toStdString().c_str();
}

QJsonObject Error::toJson() const {
    QJsonObject obj;
    obj.insert("error", (int)m_error);
    obj.insert("errorDesc", m_desc);
    return obj;
}
