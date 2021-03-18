#ifndef DATA_H
#define DATA_H

#include <QJsonObject>
#include <QString>

using bounds_t = std::pair<int, int>;

enum class EGameState : int {
    uninitialized = 0,
    InProgress,
    AutoPlay,
    Won,
    Lost,
    Abondoned
};

enum class ECompare : int {
    Smaller = 0,
    Equal,
    Bigger,
};

class Data {
public:
    enum EDataType : int {
        Initialization = 0,
        InitializationReply,
        Guess,
        GuessReply,
        ServerError,
    };

    QString toJsonString() const;

    virtual QJsonObject toJson() const = 0;
    virtual EDataType type() const = 0;

    template <typename T>
    static std::unique_ptr<T> parse(const QString& string) {
        return std::move(
            std::unique_ptr<T>(static_cast<T*>(parse_imp(string))));
    }

private:
    static Data* parse_imp(const QString& string);
};

class InitializationData : public Data {
public:
    InitializationData(const QJsonObject& json);
    InitializationData() {}
    QString m_name;
    bool m_autoplay;

public:
    // Data interface
    QJsonObject toJson() const override;
    EDataType type() const { return Initialization; }
};

class InitializationReplyData : public Data {
public:
    InitializationReplyData(const QJsonObject& json);
    InitializationReplyData() {}
    int m_maxTries;
    bounds_t m_bounds;
    EGameState m_gameState;

public:
    // Data interface
    QJsonObject toJson() const override;
    EDataType type() const { return Initialization; }
};

class GuessData : public Data {
public:
    GuessData(const QJsonObject& json);
    GuessData() {}
    int m_number;

    // Data interface
    QJsonObject toJson() const override;
    EDataType type() const { return Guess; }
};

class GuessReplyData : public Data {
public:
    GuessReplyData(const QJsonObject& json);
    GuessReplyData() {}

    ECompare m_compare;
    int m_triesLeft;
    int m_tries;
    EGameState m_gameState;

    std::optional<int> m_number;

    // Data interface
    QJsonObject toJson() const override;
    EDataType type() const { return GuessReply; }
};

#endif // DATA_H
