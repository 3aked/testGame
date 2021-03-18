#include "data.h"

#include "errors.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <exception>

QString Data::toJsonString() const {
    QJsonObject json;
    json.insert("type", (int)type());
    json.insert("data", toJson());

    QJsonDocument doc(json);
    return QString(doc.toJson(QJsonDocument::Compact));
}

Data* Data::parse_imp(const QString& string) {
    QJsonParseError err;
    Data* data = nullptr;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(string.toUtf8(), &err);
    if (err.error == QJsonParseError::NoError) {
        QJsonObject json = jsonResponse.object();
        if (json.isEmpty()) {
            throw Error(EError::BadDataRequest, "empty json");
        } else {
            if (json.contains("type")) {
                auto type = (EDataType)json.value("type").toInt();
                auto obj = json.value("data").toObject();
                if (obj.isEmpty()) {
                    throw Error(EError::BadDataRequest, "empty data json");
                }
                switch (type) {
                case Initialization:
                    return new InitializationData(obj);
                case InitializationReply:
                    return new InitializationReplyData(obj);
                case Guess:
                    return new GuessData(obj);
                case GuessReply:
                    return new GuessReplyData(obj);
                case ServerError:
                    return new Error(obj);
                default:
                    throw Error(EError::BadDataRequest, "unknown data type");
                }
            } else {
                throw Error(EError::BadDataRequest, "type missing");
            }
        }
    } else {
        throw Error(EError::BadDataRequest, err.errorString());
    }
}

InitializationData::InitializationData(const QJsonObject& json) {

    m_name = json.value("name").toString("");
    m_autoplay = json.value("autoplay").toBool(false);
}

QJsonObject InitializationData::toJson() const {
    QJsonObject obj;
    obj.insert("name", m_name);
    obj.insert("autoplay", m_autoplay);
    return obj;
}

InitializationReplyData::InitializationReplyData(const QJsonObject& json) {
    if (!json.contains("maxTries")) {
        throw Error(EError::BadDataRequest, "maxTries missing");
    } else {
        auto value = json.value("maxTries");
        if (value.isDouble()) {
            m_maxTries = value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid maxTries");
        }
    }

    if (!json.contains("bounds")) {
        throw Error(EError::BadDataRequest, "bounds missing");
    } else {
        auto value = json.value("bounds");
        if (value.isObject()) {
            if (!json.contains("min")) {
                throw Error(EError::BadDataRequest, "min missing");
            } else {
                auto value = json.value("min");
                if (value.isDouble()) {
                    m_bounds.first = value.toInt();
                } else {
                    throw Error(EError::BadDataRequest, "invalid min");
                }
            }

            if (!json.contains("max")) {
                throw Error(EError::BadDataRequest, "max missing");
            } else {
                auto value = json.value("max");
                if (value.isDouble()) {
                    m_bounds.second = value.toInt();
                } else {
                    throw Error(EError::BadDataRequest, "invalid max");
                }
            }
        } else {
            throw Error(EError::BadDataRequest, "invalid maxTries");
        }
    }
    if (!json.contains("gameState")) {
        throw Error(EError::BadDataRequest, "gameState missing");
    } else {
        auto value = json.value("gameState");
        if (value.isDouble()) {
            m_gameState = (EGameState)value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid gameState");
        }
    }
}

QJsonObject InitializationReplyData::toJson() const {

    QJsonObject obj;
    obj.insert("maxTries", m_maxTries);

    QJsonObject objBounds;
    objBounds.insert("min", m_bounds.first);
    objBounds.insert("max", m_bounds.second);
    obj.insert("bounds", objBounds);

    obj.insert("gameState", (int)m_gameState);

    return obj;
}

GuessData::GuessData(const QJsonObject& json) {

    if (!json.contains("number")) {
        throw Error(EError::BadDataRequest, "number missing");
    } else {
        auto value = json.value("number");
        if (value.isDouble()) {
            m_number = value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid number");
        }
    }
}

QJsonObject GuessData::toJson() const {
    QJsonObject obj;
    obj.insert("number", m_number);
    return obj;
}

GuessReplyData::GuessReplyData(const QJsonObject& json) {
    if (!json.contains("compare")) {
        throw Error(EError::BadDataRequest, "compare missing");
    } else {
        auto value = json.value("compare");
        if (value.isDouble()) {
            m_compare = (ECompare)value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid compare");
        }
    }

    if (!json.contains("triesLeft")) {
        throw Error(EError::BadDataRequest, "triesLeft missing");
    } else {
        auto value = json.value("triesLeft");
        if (value.isDouble()) {
            m_triesLeft = value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid triesLeft");
        }
    }
    if (!json.contains("tries")) {
        throw Error(EError::BadDataRequest, "tries missing");
    } else {
        auto value = json.value("tries");
        if (value.isDouble()) {
            m_tries = value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid tries");
        }
    }

    if (!json.contains("gameState")) {
        throw Error(EError::BadDataRequest, "gameState missing");
    } else {
        auto value = json.value("gameState");
        if (value.isDouble()) {
            m_gameState = (EGameState)value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid gameState");
        }
    }

    if (json.contains("number")) {
        auto value = json.value("number");
        if (value.isDouble()) {
            m_number = value.toInt();
        } else {
            throw Error(EError::BadDataRequest, "invalid number");
        }
    }
}

QJsonObject GuessReplyData::toJson() const {
    QJsonObject obj;
    obj.insert("compare", (int)m_compare);
    obj.insert("triesLeft", m_triesLeft);
    obj.insert("gameState", (int)m_gameState);
    obj.insert("tries", m_tries);

    if (m_number) {
        obj.insert("number", *m_number);
    }

    return obj;
}
