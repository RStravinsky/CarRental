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
    if(status) {
        QSqlQueryModel * historyTable = new QSqlQueryModel(this);
        historyTable->setQuery(QString("SELECT Name, Surname, Destination FROM history WHERE idCar = %1 AND End IS NULL").arg(idCar));

        ui->lblPerson->setText(
                    historyTable->data(historyTable->index(historyTable->rowCount()-1,0)).toString() + QString(" ") +
                    historyTable->data(historyTable->index(historyTable->rowCount()-1,1)).toString() + QString(" ") +
                    historyTable->data(historyTable->index(historyTable->rowCount()-1,2)).toString()
                    );

        delete historyTable;
    }
    else {
        ui->lblPersonImage->setVisible(false);
        ui->lblPerson->setVisible(false);
    }
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
                                   "background: rgb(255,140,0);}"
                                   "QPushButton{ color: white; border-radius: 5px;"
                                   "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                   "stop: 0 rgba(255,140,0), stop: 0.7 rgb(255,105,0));}"
                                   "QPushButton:pressed { color: white; border-radius: 5px;"
                                   "background: rgb(255,105,0);}"
                                    );
        ui->btnRent->setText("Oddaj");
    }
}

bool CarBlock::checkStatus()
{
    if(Database::isOpen()) {
        Status checkedStatus = carStatus;
        QSqlQuery qry("SELECT idCar,Status FROM car");
        while(qry.next()) {
            if(qry.value(0).toInt()==idCar) {
                checkedStatus = static_cast<CarBlock::Status>(qry.value(1).toInt());
            }
        }

        if(checkedStatus != carStatus) {
            QMessageBox::warning(this,"Uwaga!","Polecenie nie powidoło się!");
            emit statusChanged();
            return true;
        }
        else
            return false;
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
        return true;
    }
}

bool CarBlock::addToHistory(QString name, QString surname, QString destination, QString target)
{
    if(Database::isOpen()) {
        QSqlQuery qry;
        qry.prepare("INSERT INTO history (Name, Surname, Begin, idCar, Destination, Target) "
                    "VALUES (:_Name, :_Surname, :_Begin, :_idCar, :_Destination, :_Target);"
                    "UPDATE car SET Status=:_Status WHERE idCar=:_idCar");
        qry.bindValue(":_Name", name);
        qry.bindValue(":_Surname", surname);
        qry.bindValue(":_Begin", QDateTime::currentDateTime());
        qry.bindValue(":_Status", 1);
        qry.bindValue(":_idCar", idCar);
        qry.bindValue(":_Destination", destination);
        qry.bindValue(":_Target", target);
        if(!qry.exec())
            return false;
        else
            return true;
    }
    else {
        QMessageBox::critical(this,"Bład!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
        return false;
    }
}

bool CarBlock::updateHistory(QString mileage, QString notes, int distance)
{
    if(Database::isOpen()) {

        QSqlQuery qry;
        QString name, surname;
        QSqlQueryModel * historyTable = new QSqlQueryModel(this);
        historyTable->setQuery("SELECT * FROM history;");
        for(int i=0; i<historyTable->rowCount(); ++i){
            if(historyTable->data(historyTable->index(i,5)).toInt() == idCar &&
               historyTable->data(historyTable->index(i,4)).toDate().toString("yyyy-MM-dd") == ""){

                name = historyTable->data(historyTable->index(i,1)).toString();
                surname = historyTable->data(historyTable->index(i,2)).toString();

                if(notes.isEmpty()) {
                    qry.prepare("UPDATE history SET End=:_End, Distance=:_Distance WHERE idCar=:_idCar AND End IS NULL;"
                                "UPDATE car SET Mileage=:_Mileage WHERE idCar=:_idCar;"
                                "UPDATE car SET Status=:_Status WHERE idCar=:_idCar");
                    qry.bindValue(":_idCar", idCar);
                    qry.bindValue(":_End", QDateTime::currentDateTime());
                    qry.bindValue(":_Distance",distance);
                    qry.bindValue(":_Mileage", mileage.toInt());
                    qry.bindValue(":_Status", 0);
                }

                else {
                qry.prepare("INSERT INTO notes (Contents, Name, Surname, Datetime, isRead, idCar) "
                            "VALUES (:_Contents, :_Name, :_Surname, :_Datetime, :_isRead, :_idCar);"
                            "UPDATE history SET End=:_End WHERE idCar=:_idCar AND End IS NULL;"
                            "UPDATE car SET Status=:_Status WHERE idCar=:_idCar;"
                            "UPDATE car SET Mileage=:_Mileage WHERE idCar=:_idCar;");
                qry.bindValue(":_Contents", notes);
                qry.bindValue(":_Name", name);
                qry.bindValue(":_Surname", surname);
                qry.bindValue(":_Datetime", QDateTime::currentDateTime());
                qry.bindValue(":_isRead", 0);
                qry.bindValue(":_idCar", idCar);
                qry.bindValue(":_End", QDateTime::currentDateTime());
                qry.bindValue(":_Mileage", mileage.toInt());
                qry.bindValue(":_Status", 0);
                }
                if(!qry.exec())
                    return false;
                else
                    return true;
            }
        }
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
        return false;
    }
}

bool CarBlock::isReservation(QString &person)
{
    QSqlQueryModel * bookingTable = new QSqlQueryModel(this);
    bookingTable->setQuery(QString("SELECT Name,Surname, Begin, End FROM booking WHERE idCar = %1").arg(idCar));

    for(int i=0;i<bookingTable->rowCount();++i)
    {
        if((QDateTime::currentDateTime()>= bookingTable->data(bookingTable->index(i,2)).toDateTime() &&
           QDateTime::currentDateTime()<= bookingTable->data(bookingTable->index(i,3)).toDateTime()) ||
           (QDateTime::currentDateTime().addSecs(900) >= bookingTable->data(bookingTable->index(i,2)).toDateTime() &&
           QDateTime::currentDateTime()<= bookingTable->data(bookingTable->index(i,3)).toDateTime()))
        {
            person = bookingTable->data(bookingTable->index(i,0)).toString() + " " +
                     bookingTable->data(bookingTable->index(i,1)).toString();
            delete bookingTable;
            return true;
        }
    }

    delete bookingTable;
    return false;
}

void CarBlock::on_btnRent_clicked()
{
    emit inProgress();
    if(Database::isOpen()) {

        bool isChanged = checkStatus(); // check if status changed

        if(!isChanged) {
            if(carStatus == Status::Free) {

                QString person;
                if(isReservation(person))
                    if(!showMsgBeforeReserve(person)){
                        emit progressFinished();
                        return;
                    }

                NameDialog * nameDialog = new NameDialog(idCar);
                if(nameDialog->exec() == NameDialog::Accepted) {
                    isChanged = checkStatus(); // check if status changed
                    if(!isChanged) {
                        QString name,surname,destination,target;
                        nameDialog->getNameAndSurname(name,surname,destination,target);
                        if(addToHistory(name,surname,destination,target)){
                            //QMessageBox::information(this,"Informacja","Wypożyczono!");
                            emit statusChanged();
                        }
                        else
                            QMessageBox::warning(this,"Uwaga","Polecenie nie powiodło się!");
                    }
                }
                delete nameDialog;
            }
            else if(carStatus == Status::Rented) {
                ReturnDialog * returnDialog = new ReturnDialog(ui->lblMileage->text().split(" ").at(0).toInt(),idCar);
                if( returnDialog->exec() == ReturnDialog::Accepted) {
                    isChanged = checkStatus(); // check if status changed
                    if(!isChanged) {
                        if(updateHistory(returnDialog->getMileage(),returnDialog->getNotes(),returnDialog->getDistance())){
                            //QMessageBox::information(this,"Informacja","Oddano!");
                            emit statusChanged();
                        }
                        else
                            QMessageBox::warning(this,"Uwaga!","Polecenie nie powiodło się!");
                    }
                delete returnDialog;
                }
            }
        }
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
    }

    emit progressFinished();
}

bool CarBlock::showMsgBeforeReserve(QString &person)
{
    QMessageBox msgBox(QMessageBox::Question, QString("Uwaga!"), QString("<font face=""Calibri"" size=""3"" color=""gray"">Samochód jest zarezerwowany przez: "+person+". Czy na pewno chcesz wypożyczyć?</font>"), QMessageBox::Yes | QMessageBox::No );

    msgBox.setStyleSheet("QMessageBox {background: white;}"
                         "QPushButton:hover {"
                         "border-radius: 5px;"
                         "background: rgb(255,140,0);"
                         "}"
                         "QPushButton{"
                         "color: white;"
                         "border-radius: 5px;"
                         "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                         "stop: 0 rgba(255,140,0), stop: 0.7 rgb(255,105,0));"
                         "min-width: 70px;"
                         "min-height: 30px;"
                         "font-family: Calibri;"
                         "font-size: 12;"
                         "font-weight: bold;"
                         "}"
                         "QPushButton:pressed {"
                         "color: white;"
                         "border-radius: 5px;"
                         "background: rgb(255,105,0);"
                         "}"
                         );

    msgBox.setWindowIcon(QIcon(":/images/images/icon.ico"));
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Nie"));
    if (msgBox.exec() == QMessageBox::No)
        return false;

    return true;
}

