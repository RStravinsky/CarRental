#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>
#include <memory>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <namedialog.h>
#include <returndialog.h>

namespace Ui {
class CarBlock;
}

class CarBlock : public QWidget
{
    Q_OBJECT

public:
    enum Status {
        Free,
        Rented,
    } carStatus;

    explicit CarBlock(int id, QString name, QString model, QString licensePlate, int mileage, Status status = CarBlock::Free, QString photoPath = ":/images/images/car.png", QWidget *parent = 0);
    ~CarBlock();
    void setStatus(Status);
    void setBookingTable(QSqlQueryModel * bookTable) {bookingTable = bookTable;}

private slots:
    void on_btnRent_clicked();

signals:
    void statusChanged();
    void inProgress();
    void progressFinished();
    void changeStatusBar(QString,int timeout = 0);

private:
    Ui::CarBlock *ui;

    int idCar;
    QSqlQueryModel * bookingTable{nullptr};
    QSqlDatabase sqlDatabase;

    bool connectToDatabase(QString login, QString password);
    void closeDatabase();

    void setRentButton(Status status);
    bool checkStatus();
    bool addToHistory(QString name, QString surname);
    bool updateHistory(QString mileage, QString notes);
};

#endif // CARBLOCK_H
