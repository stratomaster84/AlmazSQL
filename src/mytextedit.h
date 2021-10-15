#ifndef mytextedit_H
#define mytextedit_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class MyTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    MyTextEdit(QWidget *parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent *event);   // обработчик события нажатия кнопки Shift+Enter на клавиатуре

Q_SIGNALS:
    void signalToQuerySlot();    // сигнал для связывания нажатия кнопки на клавиатуре и обработчика запроса slotQuery
};

#endif // mytextedit_H
