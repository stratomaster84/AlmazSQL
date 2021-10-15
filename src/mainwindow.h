#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase connector;    // Дескриптор подключения к БД

public slots:
    void slotConnect();     // обработчик нажатия кнопки соединения
    void slotDisconnect();  // обработчик нажатия кнопки разъединения
    void slotQuery();       // обработчик нажатия кнопки запроса
    void slotComboBoxIndexChanged(int index); // обработчик перемены драйвера БД в списке

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
