#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPair>
#include <QValidator>
#include <QRegExp>
#include <QSqlQueryModel>
#include <QCompleter>

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(int id, QWidget *parent = 0);
    ~NameDialog();
    void getNameAndSurname(QString & name, QString & surname, QString &destination, QString &target);

private slots:
    void on_pushButtonConfirm_released();

private:
    Ui::NameDialog *ui;
    int idCar;

    QSqlQueryModel * historyTable{nullptr};
    QStringList nameList;
    QStringList surnameList;
    QStringList destinationList;
    QCompleter * nameCompleter{nullptr};
    QCompleter * surnameCompleter{nullptr};
    QCompleter * destinationCompleter{nullptr};
    void setCompleterLists();
    void setCompleters();
    QString scrollBarStylesheet();
};

#endif // NAMEDIALOG_H
