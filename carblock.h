#ifndef CARBLOCK_H
#define CARBLOCK_H

#include <QWidget>
#include <QDate>
#include <bookingdialog.h>
#include <memory>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>

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

private:
    Ui::CarBlock *ui;
    int idCar;
    BookingDialog * bookingDialog{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    void setRentButton(Status status);
};

#endif // CARBLOCK_H
