#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

// конструктор формы
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // нарисовать форму

    connect(ui->connect_but, SIGNAL(clicked()),SLOT(slotConnect()));
    connect(ui->disconnect_but, SIGNAL(clicked()),SLOT(slotDisconnect()));
    connect(ui->query_but, SIGNAL(clicked()),SLOT(slotQuery()));
    connect(ui->query_text, SIGNAL(signalToQuerySlot()),SLOT(slotQuery()));
    connect(ui->DB_box, SIGNAL(currentIndexChanged(int)),SLOT(slotComboBoxIndexChanged(int)));
}

// деструктор формы
MainWindow::~MainWindow()
{
    if(connector.isOpen())
        connector.close();    // при выходе разорвать соединение, если установлено
    delete ui;
}

// обработчик нажатия кнопки соединения
void MainWindow::slotConnect(){
    QMessageBox msgBox;
    if(connector.isOpen()){
        msgBox.setText("Соединение уже установлено... Разорвите соединение!");
    }
    else{
        QString DB_type;
        if(ui->DB_box->currentIndex() != 8)
            DB_type = ui->DB_box->currentText();
        else
            DB_type = ui->DB_lbl->text();

        connector = QSqlDatabase::addDatabase(DB_type);

        if(!connector.isValid()){
            msgBox.setText("Драйвер \"" + DB_type + "\" отсутствует!");
            msgBox.exec();
            return;
        }
        connector.setHostName(ui->host_lbl->text());
        connector.setUserName(ui->user_lbl->text());
        connector.setPassword(ui->password_lbl->text());
        connector.setDatabaseName(ui->database_lbl->text());
        //connector.setPort(3306 /*для MySQL*/);
        //connector.setConnectOptions("");
        //connector.setNumericalPrecisionPolicy();

        if(!connector.open()){
            QString _err = "";
            bool _first = true;
            if(connector.lastError().databaseText()!=""){
                _err += "ОШИБКА БАЗЫ: " + connector.lastError().databaseText();
                _first = false;
            }
            if(connector.lastError().driverText()!=""){
                if(!_first)
                    _err += "\n";
                _err += "ОШИБКА ДРАЙВЕРА: " + connector.lastError().driverText();
            }
            msgBox.setText(_err);
        }
        else{           // если соединение удалось
            msgBox.setText("Соединение установлено!");
        }
    }
    msgBox.exec();
}

// обработчик нажатия кнопки разъединения
void MainWindow::slotDisconnect(){
    QMessageBox msgBox;
    if(connector.isOpen()){                    // если соединение установлено
        connector.close();    // разорвать соединение
        msgBox.setText("Соединение разорвано!");   // и вывести сообщение
    }
    else{                           // иначе вывести сообщение
        msgBox.setText("Соединение не установлено!");
    }
    msgBox.exec();
}

// обработчик нажатия кнопки запроса
void MainWindow::slotQuery(){
    QMessageBox msgBox;
    if(!connector.isOpen()){               // если соединение не установлено - вывести сообщение
        msgBox.setText("Соединение не установлено!");
        msgBox.exec();
        return;
    }

// ВЫПОЛНЕНИЕ САМОГО ЗАПРОСА И ВЫВОД РЕЗУЛЬТАТА
    QSqlQuery query;
    if(ui->query_text->toPlainText().isEmpty()){
        msgBox.setText("Введён пустой запрос!");
        msgBox.exec();
    }
    else if(!query.exec(ui->query_text->toPlainText())){
        // ЕСЛИ ОШИБКА В ЗАПРОСЕ
        QString _err = "";
        bool _first = true;
        if(query.lastError().databaseText()!=""){
            _err += "ОШИБКА БАЗЫ: " + query.lastError().databaseText();
            _first = false;
        }
        if(query.lastError().driverText()!=""){
            if(!_first)
                _err += "\n";
            _err += "ОШИБКА ДРАЙВЕРА: " + query.lastError().driverText();
        }
        msgBox.setText(_err);
        msgBox.exec();
    }
    else if(!query.isSelect()){
        msgBox.setText("КОМАНДА:\n" + query.lastQuery() +
                       "\nВЫПОЛНЕНА УСПЕШНО!");
        msgBox.exec();
    }
    else if(query.size() == 0){
        msgBox.setText("Результат пуст!");
        msgBox.exec();
    }
    else{
// СДЕЛАН ВЕРНЫЙ SELECT и ИМЕЕТ НЕ ПУСТОЙ РЕЗУЛЬТАТ
        int i;
        QSqlRecord rec = query.record();
        int numRecords = query.size();
        int numFields = rec.count();
        ui->result_table->clear();
        ui->result_table->setRowCount(numRecords);
        ui->result_table->setColumnCount(numFields);
        ui->result_table->verticalHeader()->hide(); // заголовки строк не нужны

        QStringList _hdrs;
        for(i = 0;i<numFields;i++)
        {
            _hdrs.append(rec.fieldName(i));          // сохранить его имя
        }
        ui->result_table->setHorizontalHeaderLabels(_hdrs); // инициализировать заголовки таблицы

        QTableWidgetItem* ptwi = 0; // указатель на новый элемент таблицы
        for(i = 0;i<numRecords;i++)
        {
            // получить очередную строку
            query.next();
            int j;
            for(j=0;j<numFields;j++){
                ptwi = new QTableWidgetItem(query.value(j).toString());    // создать новый элемент с текстом
                ui->result_table->setItem(i, j, ptwi);  // и послать его в соответствующую ячейку таблицы
            }
        }
        ui->result_table->resizeColumnsToContents();    // подогнать содержимое по ширине...
        ui->result_table->resizeRowsToContents();       // ...и по высоте
    }
}

void MainWindow::slotComboBoxIndexChanged(int index)
{
    if(index == 8)
        ui->DB_lbl->setEnabled(true);
    else
        ui->DB_lbl->setEnabled(false);
}
