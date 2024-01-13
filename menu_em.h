#ifndef MENU_EM_H
#define MENU_EM_H
#include <QWidget>
#include "order.h"
#include "goods.h"
#include "user.h"

namespace Ui {
class menu_em;
}

class menu_em : public QWidget
{
    Q_OBJECT

public:
    explicit menu_em(QWidget *parent = nullptr);
    ~menu_em();
private slots:
    void slot_ret();
    void get_name(QString);
    void into_goods();
    void into_user();
    void into_order();
    void reshow();

signals:
    void signal_ret();

private:
    Ui::menu_em *ui;
    QString user_name;

};

#endif // MENU_EM_H
