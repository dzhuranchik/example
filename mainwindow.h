#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logger.h" // Подключаем наш логгер

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Переопределяем событие закрытия окна
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Слот, который будет вызываться при нажатии на кнопку
    void on_button_clicked();

    // Слот для обработки выбора элемента в dropdown list
    void on_dropdownList_currentIndexChanged(int index);

private:
    // Метод для первоначальной настройки виджетов
    void setupInitialState();

    // Метод для заполнения dropdown list случайными строками
    void populateDropdownList();

    // Метод для сохранения элементов списка в файл
    void saveDropdownListItems();

    Ui::MainWindow *ui; // Указатель на объект интерфейса
    int m_clickCount;   // Счетчик нажатий на кнопку
};
#endif // MAINWINDOW_H
