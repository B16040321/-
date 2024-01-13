#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <menu_admin.h>
#include "menu_em.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

signals:
    void send_name(QString);
    void signal_ret();

private slots:

    void on_pwdLineEdit_returnPressed();

    void on_loginBtn_clicked();

    void on_quitBtn_clicked();

    void on_showPwdBtn_clicked();

    void on_pwdLineEdit_textChanged(const QString &str);

    void reshow();
    void clearUI();

private:
    Ui::LoginDialog *ui;
    menu_admin *m_a;
    menu_em *m_e;
};

#endif // LOGINDIALOG_H
