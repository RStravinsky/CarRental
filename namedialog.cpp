#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog),
    idCar(id)
{
    ui->setupUi(this);
    connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
}

NameDialog::~NameDialog()
{
    delete ui;
}

void NameDialog::getNameAndSurname(QString &name, QString &surname)
{
    name = ui->lineEditName->text();
    surname = ui->lineEditSurname->text();
}

void NameDialog::on_pushButtonConfirm_released()
{
    QString name = ui->lineEditName->text();
    QString surname = ui->lineEditSurname->text();

    if(name.isEmpty() || surname.isEmpty())
        QMessageBox::information(this,"Informacja","Pole tekstowe nie zostało wypełnione.");

    else this->accept();
}
