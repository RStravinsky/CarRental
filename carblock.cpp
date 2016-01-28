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
    ui->lblMileage->setText(QString::number(mileage) + " km");
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

bool CarBlock::checkStatus()
{
    Status checkedStatus;
    QSqlQuery qry("SELECT Status FROM car");
    int i=1;
    while(qry.next()) {
        if(i==idCar)
            checkedStatus = static_cast<CarBlock::Status>(qry.value(0).toInt());
        i++;
    }

    if(!(checkedStatus != carStatus))
        return false;
    else
        return true;
}

bool CarBlock::addToHistory(QString name, QString surname)
{
    QSqlQuery qry;
    qry.prepare("INSERT INTO history (Name, Surname, Begin, idCar) "
                "VALUES (:_Name, :_Surname, :_Begin, :_idCar);"
                "UPDATE car SET Status=:_Status WHERE idCar=:_idCar");
    qry.bindValue(":_Name", name);
    qry.bindValue(":_Surname", surname);
    qry.bindValue(":_Begin", QDateTime::currentDateTime());
    qry.bindValue(":_Status", 1);
    qry.bindValue(":_idCar", idCar);
    if( !qry.exec() )
        return false;

    return true;
}

bool CarBlock::updateHistory(QString mileage, QString notes)
{
    QSqlQuery qry;
    QString name, surname;

    if(notes.isEmpty()) {
        qry.prepare("UPDATE history SET End=:_End WHERE idCar=:_idCar;"
                    "UPDATE car SET Mileage=:_Mileage WHERE idCar=:_idCar;"
                    "UPDATE car SET Status=:_Status WHERE idCar=:_idCar");
        qry.bindValue(":_idCar", idCar);
        qry.bindValue(":_End", QDateTime::currentDateTime());
        qry.bindValue(":_Mileage", mileage);
        qry.bindValue(":_Status", 0);
    }
    else {
        QSqlQueryModel * historyTable = new QSqlQueryModel(this);
        historyTable->setQuery("SELECT * FROM history;");
        for(int i=0; i<historyTable->rowCount(); ++i){
            if(historyTable->data(historyTable->index(i,5)).toInt() == idCar &&
               historyTable->data(historyTable->index(i,4)).toString().isEmpty()){
                name = historyTable->data(historyTable->index(i,1)).toString();
                surname = historyTable->data(historyTable->index(i,2)).toString();
            }

        }

        qry.prepare("INSERT INTO notes (Contents, Name, Surname, Datetime, isRead, idCar) "
                    "VALUES (:_Contents, :_Name, :_Surname, :_Datetime, :_isRead, :_idCar);"
                    "UPDATE history SET End=:_End WHERE idCar=:_idCar;"
                    "UPDATE car SET Mileage=:_Mileage WHERE idCar=:_idCar;"
                    "UPDATE car SET Status=:_Status WHERE idCar=:_idCar;");
        qry.bindValue(":_Contents", notes);
        qry.bindValue(":_Name", name);
        qry.bindValue(":_Surname", surname);
        qry.bindValue(":_Datetime", QDateTime::currentDateTime());
        qry.bindValue(":_isRead", 0);
        qry.bindValue(":_idCar", idCar);
        qry.bindValue(":_End", QDateTime::currentDateTime());
        qry.bindValue(":_Mileage", mileage);
        qry.bindValue(":_Status", 0);
    }
    if( !qry.exec() ){
        qDebug() << qry.lastError().text() << endl;
        return false;
    }

    return true;
}

void CarBlock::on_btnRent_clicked()
{ 
    emit inProgress();

    bool isChanged = checkStatus(); // check if status changed

    if(!isChanged) {
        if(carStatus == Status::Free) {
            NameDialog * nameDialog = new NameDialog(idCar);
            if(nameDialog->exec() == NameDialog::Accepted) {
                isChanged = checkStatus(); // check if status changed
                if(!isChanged) {
                    QString name,surname;
                    nameDialog->getNameAndSurname(name,surname);
                    if(addToHistory(name,surname)){
                        QMessageBox::information(this,"Informacja","Wypożyczono!");
                        emit statusChanged();
                    }
                    else
                        QMessageBox::warning(this,"Uwaga","Polecenie nie powiodło się!");
                }
            }
            delete nameDialog;
        }
        else if(carStatus == Status::Rented) {
            ReturnDialog * returnDialog = new ReturnDialog(idCar);
            if( returnDialog->exec() == ReturnDialog::Accepted) {
                isChanged = checkStatus(); // check if status changed
                if(!isChanged) {
                    if(updateHistory(returnDialog->getMileage(),returnDialog->getNotes())){
                        QMessageBox::information(this,"Informacja","Oddano!");
                        emit statusChanged();
                    }
                    else
                        QMessageBox::warning(this,"Uwaga","Polecenie nie powiodło się!");
                }
            delete returnDialog;
            }
        }
    }
    else {
        QMessageBox::warning(this,"Informacja","Polecenie nie powidoło się!");
        emit statusChanged();
    }

    emit progressFinished();
}
