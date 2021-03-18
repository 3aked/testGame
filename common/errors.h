#ifndef ERRORS_H
#define ERRORS_H

#include "data.h"

#include <QString>

enum class EError { BadDataRequest = 0 };

class Error : public Data, public std::exception {
public:
    Error(EError error, const QString& desc = "");
    Error(const QJsonObject& json);

    QString errorString() const;

    EError m_error;

    QString m_desc;

    // std::exception interface
    const char* what() const noexcept;

    // Data interface
    QJsonObject toJson() const override;
    EDataType type() const override { return ServerError; }
};

#endif // ERRORS_H
