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

Data *Data::parse_imp(const QString &string) {
  QJsonParseError err;
  QJsonDocument jsonResponse = QJsonDocument::fromJson(string.toUtf8(), &err);
  if (err.error == QJsonParseError::NoError) {
    QJsonObject json = jsonResponse.object();
    if (json.isEmpty()) {
      throw ErrorData(EError::BadData, "empty json");
    } else {
      if (json.contains("type")) {
        auto type = (EDataType)json.value("type").toInt();
        auto obj = json.value("data").toObject();
        if (obj.isEmpty()) {
          throw ErrorData(EError::BadData, "empty data json");
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
        case Error:
          return new ErrorData(obj);
        default:
          throw ErrorData(EError::BadData, "unknown data type");
        }
      } else {
        throw ErrorData(EError::BadData, "type missing");
      }
    }
  } else {
    throw ErrorData(EError::BadData, err.errorString());
  }
}

InitializationData::InitializationData(const QJsonObject &json) {

  m_name = json.value("name").toString("");
}

QJsonObject InitializationData::toJson() const {
  QJsonObject obj;
  obj.insert("name", m_name);
  return obj;
}

InitializationReplyData::InitializationReplyData(const QJsonObject &json) {
  if (!json.contains("maxTries")) {
    throw ErrorData(EError::BadData, "maxTries missing");
  } else {
    auto value = json.value("maxTries");
    if (value.isDouble()) {
      m_maxTries = value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid maxTries");
    }
  }

  if (!json.contains("bounds")) {
    throw ErrorData(EError::BadData, "bounds missing");
  } else {
    auto value = json.value("bounds");
    if (value.isObject()) {
      auto jsonBounds = value.toObject();
      if (!jsonBounds.contains("min")) {
        throw ErrorData(EError::BadData, "min missing");
      } else {
        auto value = jsonBounds.value("min");
        if (value.isDouble()) {
          m_bounds.first = value.toInt();
        } else {
          throw ErrorData(EError::BadData, "invalid min");
        }
      }

      if (!jsonBounds.contains("max")) {
        throw ErrorData(EError::BadData, "max missing");
      } else {
        auto value = jsonBounds.value("max");
        if (value.isDouble()) {
          m_bounds.second = value.toInt();
        } else {
          throw ErrorData(EError::BadData, "invalid max");
        }
      }
    } else {
      throw ErrorData(EError::BadData, "invalid maxTries");
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

  return obj;
}

GuessData::GuessData(const QJsonObject &json) {

  if (!json.contains("number")) {
    throw ErrorData(EError::BadData, "number missing");
  } else {
    auto value = json.value("number");
    if (value.isDouble()) {
      m_number = value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid number");
    }
  }
}

QJsonObject GuessData::toJson() const {
  QJsonObject obj;
  obj.insert("number", m_number);
  return obj;
}

GuessReplyData::GuessReplyData(const QJsonObject &json) {
  if (!json.contains("compare")) {
    throw ErrorData(EError::BadData, "compare missing");
  } else {
    auto value = json.value("compare");
    if (value.isDouble()) {
      m_compare = (ECompare)value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid compare");
    }
  }

  if (!json.contains("triesLeft")) {
    throw ErrorData(EError::BadData, "triesLeft missing");
  } else {
    auto value = json.value("triesLeft");
    if (value.isDouble()) {
      m_triesLeft = value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid triesLeft");
    }
  }
  if (!json.contains("tries")) {
    throw ErrorData(EError::BadData, "tries missing");
  } else {
    auto value = json.value("tries");
    if (value.isDouble()) {
      m_tries = value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid tries");
    }
  }

  if (!json.contains("gameState")) {
    throw ErrorData(EError::BadData, "gameState missing");
  } else {
    auto value = json.value("gameState");
    if (value.isDouble()) {
      m_gameState = (EGameState)value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid gameState");
    }
  }

  if (json.contains("number")) {
    auto value = json.value("number");
    if (value.isDouble()) {
      m_number = value.toInt();
    } else {
      throw ErrorData(EError::BadData, "invalid number");
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

QString toString(ECompare comp) {
  switch (comp) {
  case ECompare::Bigger:
    return "Bigger";
  case ECompare::Equal:
    return "Equal, bravo!";
  case ECompare::Smaller:
    return "Smaller";
  default:
    assert(false);
  }
  return "";
}

QString toString(EGameState state) {
  switch (state) {
  case EGameState::Abondoned:
    return "Abondoned";
  case EGameState::InProgress:
    return "InProgress";
  case EGameState::Lost:
    return "Lost";
  case EGameState::uninitialized:
    return "uninitialized";
  case EGameState::Won:
    return "Won";
  default:
    assert(false);
  }
  return "";
}
