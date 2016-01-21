#include "carblock.h"
#include "ui_carblock.h"

CarBlock::CarBlock(int id, QString name, QString model, QString licensePlate, int mileage, Status status, QString photoPath,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarBlock),
    idCar(id)
{
    ui->setupUi(this);

    ui->lblLicensePlate->setText(licensePlate);
    ui->lblPhoto->setPixmap(QPixmap(photoPath));
    ui->lblCarName->setText(name + QString(" ") + model);
    setStatus(status);
}

CarBlock::~CarBlock()
{
    delete ui;
}

void CarBlock::setStatus(Status status)
{
    if(status == Status::Free)
        ui->lblStatus->setPixmap(QPixmap(":/images/images/free.png"));
    if(status == Status::Rented)
        ui->lblStatus->setPixmap(QPixmap(":/images/images/rented.png"));
    setRentButton(status);
    carStatus = status;
}

void CarBlock::setRentButton(Status status)
{
    if(status == Status::Free) {
        ui->btnRent->setStyleSheet("QPushButton:hover { border-radius: 5px;"
                                   "background: rgb(0,190,0);}"
                                   "QPushButton{ color: white; border-radius: 5px;"
                                   "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                   "stop: 0 rgba(0,190,0), stop: 0.7 rgb(0,150,0));}"
                                   "QPushButton:pressed { color: white; border-radius: 5px;"
                                   "background: rgb(0,150,0);}"
                                    );
        ui->btnRent->setText("Wypożycz");
    }

    else if(status == Status::Rented) {
        ui->btnRent->setStyleSheet("QPushButton:hover { border-radius: 5px;"
                                   "background: rgb(240,0,0);}"
                                   "QPushButton{ color: white; border-radius: 5px;"
                                   "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                   "stop: 0 rgba(240,0,0), stop: 0.7 rgb(210,0,0));}"
                                   "QPushButton:pressed { color: white; border-radius: 5px;"
                                   "background: rgb(210,0,0);}"
                                    );
        ui->btnRent->setText("Oddaj");
    }

}

void CarBlock::on_btnRent_clicked()
{ 
    // check status
    Status checkedStatus;
    QSqlQuery qry("SELECT Status FROM car");
    uint i=1;
    while(qry.next()) {
        if(i==idCar)
            checkedStatus = static_cast<CarBlock::Status>(qry.value(0).toInt());
        i++;
    }

    if(!(checkedStatus != carStatus)) {
        if(carStatus == Status::Free) {
            qry.prepare("UPDATE car SET Status=:_Status WHERE idCar=:_id");
            qry.bindValue(":_id", idCar);
            qry.bindValue(":_Status", 1);
            if( !qry.exec() )
                QMessageBox::warning(this,"Informacja","Polecenie nie powidoło się.");
            else {
                QMessageBox::information(this,"Informacja","Wypożyczono!");\
                emit statusChanged();
            }
        }
        else if(carStatus == Status::Rented) {
            qry.prepare("UPDATE car SET Status=:_Status WHERE idCar=:_id");
            qry.bindValue(":_id", idCar);
            qry.bindValue(":_Status", 0);
            if( !qry.exec() )
                QMessageBox::warning(this,"Informacja","Polecenie nie powidoło się.");
            else {
                QMessageBox::information(this,"Informacja","Oddano!");
                emit statusChanged();
            }
        }
    }
    else {
        QMessageBox::warning(this,"Informacja","Polecenie nie powidoło się.");
        emit statusChanged();
    }

}
