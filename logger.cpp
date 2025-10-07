#include "logger.h"
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent> // Для асинхронного выполнения

// Реализация паттерна Singleton
Logger& Logger::instance()
{
    static Logger loggerInstance;
    return loggerInstance;
}

// Конструктор класса
Logger::Logger(QObject *parent) : QObject(parent)
{
    // Формируем имя файла лога при создании объекта
    m_logFileName = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss") + "-APP-LOG.txt";
}

// Деструктор
Logger::~Logger()
{

}

// Основной метод для логирования
void Logger::log(LogType type, int code, const QString& message)
{
    // Получаем текущее время с миллисекундами
    QString currentTime = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh:mm:ss.zzz");

// Определяем название сборки (DEBUG или RELEASE)
#ifdef QT_DEBUG
    const QString buildType = "DEBUG";
#else
    const QString buildType = "RELEASE";
#endif

    // Определяем тип лога
    QString logTypeStr;
    switch (type)
    {
    case LogType::INFO:
        logTypeStr = "INFO";
        break;
    case LogType::ERROR:
        logTypeStr = "ERROR";
        break;
    }

    // Формируем финальное сообщение для лога
    QString formattedMessage = QString("[%1][%2][%3][%4] - %5")
                                   .arg(currentTime)
                                   .arg(buildType)
                                   .arg(logTypeStr)
                                   .arg(code)
                                   .arg(message);

// В режиме DEBUG выводим лог в консоль
#ifdef QT_DEBUG
    qDebug() << formattedMessage;
#endif

    QtConcurrent::run([this, formattedMessage]()
                      {
                          writeToFile(formattedMessage);
                      });
}

// Метод для записи в файл
void Logger::writeToFile(const QString& formattedMessage)
{
    // Блокируем мьютекс, чтобы избежать одновременной записи из разных потоков
    QMutexLocker locker(&m_mutex);

    // Открываем файл для добавления записи
    QFile logFile(m_logFileName);
    if (logFile.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&logFile);
        out << formattedMessage << "\n"; // Записываем сообщение и перенос строки
        logFile.close();
    } else
    {
        // Если не удалось открыть файл, выводим ошибку в консоль
        qWarning() << "Could not open log file for writing:" << m_logFileName;
    }
}
