#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QThread>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_clickCount(0) // Инициализируем счетчик нажатий
{
    ui->setupUi(this); // Инициализируем интерфейс, созданный в .ui файле
    setupInitialState(); // Вызываем метод для начальной настройки
}

MainWindow::~MainWindow()
{
    delete ui; // Освобождаем память от объекта интерфейса
}

// Метод для начальной настройки
void MainWindow::setupInitialState()
{
    // Устанавливаем начальное значение progress bar
    ui->progressBar->setValue(0);
    // Устанавливаем диапазон для progress bar
    ui->progressBar->setRange(0, 100);
    populateDropdownList(); // Заполняем выпадающий список
}

// Метод для заполнения dropdown list
void MainWindow::populateDropdownList()
{
    QStringList items;
    items << "Альфа" << "Бета" << "Гамма" << "Дельта" << "Эпсилон";
    ui->dropdownList->addItems(items);
}


// Слот, вызываемый при нажатии на кнопку
void MainWindow::on_button_clicked()
{
    m_clickCount++; // Увеличиваем счетчик нажатий

    // Логируем нажатие кнопки
    Logger::instance().log(LogType::INFO, 200, QString("Нажатие на кнопку. Счетчик: %1").arg(m_clickCount));

    // Блокируем кнопку на время выполнения операции
    ui->button->setEnabled(false);
    // Сбрасываем progress bar в 0
    ui->progressBar->setValue(0);


    // Логика для 3-го нажатия
    if (m_clickCount == 3)
    {
        // Генерируем случайный процент
        int randomPercentage = QRandomGenerator::global()->bounded(100); // от 0 до 99

        // Логируем ошибку интерфейса (блокировка кнопки)
        Logger::instance().log(LogType::ERROR, 400, "Кнопка заблокирована после 3-го нажатия.");

        // Логируем ошибку программы (случайный процент)
        Logger::instance().log(LogType::ERROR, 500, QString("Имитация ошибки программы. Progress bar установлен на случайное значение: %1%").arg(randomPercentage));

        // Устанавливаем случайное значение и выходим из функции
        ui->progressBar->setValue(randomPercentage);
        return; // Кнопка останется заблокированной
    }

    // Основная логика для 1-го и 2-го нажатий (асинхронное заполнение)

    // Создаем таймер для плавного заполнения
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]()
            {
                int currentValue = ui->progressBar->value();
                if (currentValue < 50)
                {
                    ui->progressBar->setValue(currentValue + 1);
                    // Логируем текущее состояние progress bar
                    Logger::instance().log(LogType::INFO, 200, QString("Progress bar заполняется: %1%").arg(currentValue + 1));
                } else
                {
                    // Когда достигли 50%, останавливаем таймер
                    timer->stop();
                    timer->deleteLater(); // Удаляем таймер, чтобы избежать утечек памяти

                    // Показываем сообщение об ошибке
                    QMessageBox::critical(this, "Ошибка", "Не удалось выполнить действие!");

                    // Снова делаем кнопку доступной
                    ui->button->setEnabled(true);
                }
            });

    // Запускаем таймер. 3000 мс / 50 шагов = 60 мс на шаг
    timer->start(60);
}

// Слот для обработки выбора элемента в dropdown list
void MainWindow::on_dropdownList_currentIndexChanged(int index)
{
    // Проверяем, что выбран реальный элемент (индекс > -1)
    if (index > -1)
    {
        QString selectedText = ui->dropdownList->currentText();

        // Логируем выбор элемента
        Logger::instance().log(LogType::INFO, 100, QString("Выбран элемент из списка: '%1'").arg(selectedText));

        // Показываем всплывающее окно
        QMessageBox::information(this, "Выбор сделан", "Вы выбрали: " + selectedText);
    }
}

// Переопределенный метод, который вызывается при закрытии окна
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveDropdownListItems(); // Сохраняем данные перед выходом
    QMainWindow::closeEvent(event); // Вызываем реализацию базового класса
}

// Метод для сохранения элементов списка в файл
void MainWindow::saveDropdownListItems()
{
    QFile file("temp_dropdownlist_info.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (int i = 0; i < ui->dropdownList->count(); ++i)
        {
            out << ui->dropdownList->itemText(i) << "\n";
        }
        file.close();
        // Логируем успешное сохранение
        Logger::instance().log(LogType::INFO, 100, "Данные из dropdown list сохранены в файл temp_dropdownlist_info.txt.");
    } else
    {
        // Логируем ошибку, если не удалось открыть файл
        Logger::instance().log(LogType::ERROR, 500, "Не удалось открыть файл temp_dropdownlist_info.txt для записи.");
    }
}
