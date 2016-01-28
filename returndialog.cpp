#include "returndialog.h"
#include "ui_returndialog.h"

ReturnDialog::ReturnDialog(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReturnDialog),
    idCar(id)
{
    ui->setupUi(this);
    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
}

ReturnDialog::~ReturnDialog()
{
    delete ui;
}

QString ReturnDialog::getMileage()
{
    return ui->lblMileage->text();
}

QString ReturnDialog::getNotes()
{
    return ui->textEdit->toPlainText();
}

void ReturnDialog::on_pushButtonConfirm_clicked()
{
    QString mileage = ui->lblMileage->text();

    if(mileage.isEmpty())
        QMessageBox::information(this,"Informacja","Nie uzupełniono przebiegu.");

    else this->accept();
}
