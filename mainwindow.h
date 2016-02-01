#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QtSql/QSqlQueryModel>
#include <QTimer>
#include <QScrollBar>
#include <QPushButton>
#include <vector>
#include "carblock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateView();

private slots:
    void onTimerOverflow();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *scrollLayout{nullptr};
    QWidget *scrollWidget{nullptr};
    QSqlDatabase sqlDatabase;
    QSqlQueryModel * carTable{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    std::vector<CarBlock*> carBlockVector;
    QString login;
    QString password;
    QTimer * timer{nullptr};

    bool connectToDatabase(QString &login, QString &password);
    void closeDatabase();

    void createUpdateButton();
};

#endif // MAINWINDOW_H
