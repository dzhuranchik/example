#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QDateTime>

// Перечисление для типов логов
enum class LogType
{
    INFO
    , ERROR
};

class Logger : public QObject
{
    Q_OBJECT

public:
    // Статический метод для получения единственного экземпляра класса (Singleton)
    static Logger& instance();

    // Метод для записи лога
    void log(LogType type, int code, const QString& message);

private:
    // Приватный конструктор, чтобы предотвратить создание экземпляров извне
    explicit Logger(QObject *parent = nullptr);
    ~Logger(); // Деструктор

    // Запрещаем копирование и присваивание
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Метод для асинхронной записи в файл
    void writeToFile(const QString& formattedMessage);

    QString m_logFileName; // Имя файла лога
    QMutex m_mutex;        // Мьютекс для обеспечения потокобезопасности
};

#endif // LOGGER_H
