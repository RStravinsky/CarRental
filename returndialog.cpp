#include "returndialog.h"
#include "ui_returndialog.h"

ReturnDialog::ReturnDialog(int mileage, int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReturnDialog),
    idCar(id),
    previousMileage(mileage)
{
    ui->setupUi(this);
    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
    ui->lblMileage->setValidator(new QIntValidator());
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

    if(mileage.isEmpty()) {
        QMessageBox::warning(this,"Uwaga!","Nie uzupełniono przebiegu.");
        return;
    }

    if( ui->lblMileage->text().toInt() < previousMileage){
        QMessageBox::warning(this,"Uwaga!","Wpisany przebieg jest mniejszy od poprzedniego.");
        return;
    }

    this->accept();
}
