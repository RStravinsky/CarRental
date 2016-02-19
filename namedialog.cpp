#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog),
    idCar(id)
{
    ui->setupUi(this);
    //connect(ui->pushButtonCancel, SIGNAL(released()), this, SLOT(reject()));
}

NameDialog::~NameDialog()
{
    delete ui;
}

void NameDialog::getNameAndSurname(QString &name, QString &surname, QString &destination,QString &target)
{
    name = ui->lineEditName->text();
    surname = ui->lineEditSurname->text();
    destination = ui->lineEditDestination->text();
    target = ui->lineEditTarget->text();
}

void NameDialog::on_pushButtonConfirm_released()
{
    QString name = ui->lineEditName->text();
    QString surname = ui->lineEditSurname->text();
    QString destination = ui->lineEditDestination->text();
    QString target = ui->lineEditTarget->text();

    if(name.isEmpty() || surname.isEmpty() || destination.isEmpty() || target.isEmpty())
        QMessageBox::warning(this,"Uwaga!","Pole tekstowe nie zostało wypełnione.");

    else this->accept();
}
