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
#include "database.h"
#include "dbconfigdialog.h"

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
    QSqlQueryModel * carTable{nullptr};
    QSqlQueryModel * bookingTable{nullptr};
    std::vector<CarBlock*> carBlockVector;
    QTimer * timer{nullptr};
    QPushButton * dbConfigButton{nullptr};
    void createUpdateButton();
    void createDBConfigButton();
};

#endif // MAINWINDOW_H
