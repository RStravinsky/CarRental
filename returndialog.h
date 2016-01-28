#ifndef RETURNDIALOG_H
#define RETURNDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ReturnDialog;
}

class ReturnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReturnDialog(int id,QWidget *parent = 0);
    ~ReturnDialog();
    QString getMileage();
    QString getNotes();

private slots:
    void on_pushButtonConfirm_clicked();

private:
    Ui::ReturnDialog *ui;
    int idCar;
};

#endif // RETURNDIALOG_H
