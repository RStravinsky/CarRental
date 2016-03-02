#ifndef DBCONFIGDIALOG_H
#define DBCONFIGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QProcess>
#include "database.h"

namespace Ui {
class DBConfigDialog;
}

class DBConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBConfigDialog(const QString &parameters, QWidget *parent = 0);
    ~DBConfigDialog();
    static bool readFromFile(QString &line);

signals:
    void connectedToDB();
    void changeStatusBar(QString,int timeout = 0);

private slots:
    void on_runButton_clicked();

private:
    Ui::DBConfigDialog *ui;
    bool noDataBase;
    bool writeToFile(const QString &hostname, int port, const QString &database, const QString &user, const QString &password);
    bool dataIsEmpty();
    void showMsg(const QString &msg);
};

#endif // DBCONFIGDIALOG_H

