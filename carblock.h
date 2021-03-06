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
#include <database.h>

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
    void setRentButton(Status status);
    bool checkStatus();
    bool addToHistory(QString name, QString surname, QString destination, QString target);
    bool updateHistory(QString mileage, QString notes, int distance);
    bool isReservation(QString &person);
    bool showMsgBeforeReserve(QString &person);
};

#endif // CARBLOCK_H
