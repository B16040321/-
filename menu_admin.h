#ifndef MENU_ADMIN_H
#define MENU_ADMIN_H
#include <QWidget>
#include "employee.h"
#include "order.h"
#include "goods.h"
#include "user.h"

namespace Ui {
class menu_admin;
}

class menu_admin : public QWidget
{
    Q_OBJECT

public:
    explicit menu_admin(QWidget *parent = nullptr);
    ~menu_admin();
    QString user_name;

private slots:
    void slot_ret();
    void get_name(QString);
    void into_employee();
    void into_goods();
    void into_user();
    void into_order();
    void reshow();

signals:
    void signal_ret();

private:
    Ui::menu_admin *ui;
};

#endif // MENU_ADMIN_H
