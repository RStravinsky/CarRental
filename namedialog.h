#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPair>

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(int id, QWidget *parent = 0);
    ~NameDialog();
    void getNameAndSurname(QString & name, QString & surname);

private slots:
    void on_pushButtonConfirm_released();

private:
    Ui::NameDialog *ui;
    int idCar;
};

#endif // NAMEDIALOG_H
