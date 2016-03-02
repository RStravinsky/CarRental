#include "mainwindow.h"
#include "ui_mainwindow.h"

#define UPDATE_TIME 120000 // 2 minutes

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createUpdateButton();
    createDBConfigButton();
    ui->statusBar->setStyleSheet("background: white; color: gray; font-family: Calibri; font-size: 10pt;");

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimerOverflow()));
    onTimerOverflow();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onTimerOverflow()
{
    updateView();
    timer->start(UPDATE_TIME);
}

bool MainWindow::isConnectedToNetwork()
{
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    connect(&manager,SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
    QNetworkRequest request(QUrl(QString("http://google.com/")));
    QNetworkReply * reply = manager.get(request);
    eventLoop.exec();

    if(reply->error() == QNetworkReply::NoError)
        return true;

    return false;
}

void MainWindow::createDBConfigButton()
{
    dbConfigButton = new QPushButton(this);
    dbConfigButton->setIcon(QIcon(":/images/images/dbconfig.png"));
    dbConfigButton->setToolTip("Konfiguracja bazy danych");
    dbConfigButton->setStyleSheet("border:none; color: gray");
    ui->statusBar->addPermanentWidget(dbConfigButton);
    connect(dbConfigButton, &QPushButton::clicked,[=](){
        QString line{};
        timer->stop();
        if(!DBConfigDialog::readFromFile(line))
            return;
        DBConfigDialog d(line);
        connect(&d,SIGNAL(connectedToDB()),this,SLOT(updateView()),Qt::QueuedConnection);
        connect(&d,SIGNAL(changeStatusBar(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
        d.exec();
        timer->start(UPDATE_TIME);
    });
}

void MainWindow::updateView()
{
    qDebug() << "Updating ...";
    if(Database::connectToDatabase()) {
        qDebug() << "isOpen ...";
        ui->statusBar->showMessage("Połączono z bazą danych");
        const int varticalPosition = ui->scrollArea->verticalScrollBar()->value();

        delete carTable;
        delete bookingTable;
        delete scrollLayout;
        delete scrollWidget;

        carTable = new QSqlQueryModel(this);
        carTable->setQuery("SELECT * FROM car;");
        bookingTable = new QSqlQueryModel(this);
        bookingTable->setQuery("SELECT * FROM booking;");
        carBlockVector.clear();

        CarBlock * lastCarBlock{nullptr};
        for(int i = 0; i < carTable->rowCount(); ++i) {
            if(carTable->data(carTable->index(i,9)).toBool()) {
                carBlockVector.emplace_back(std::move(new CarBlock(carTable->data(carTable->index(i,0)).toInt(),
                                                                   carTable->data(carTable->index(i,1)).toString(), carTable->data(carTable->index(i,2)).toString(),
                                                                   carTable->data(carTable->index(i,3)).toString(), carTable->data(carTable->index(i,6)).toInt(),
                                                                   static_cast<CarBlock::Status>(carTable->data(carTable->index(i,7)).toInt()),
                                                                   carTable->data(carTable->index(i,8)).toString()
                                                                  )
                                                      ));
               lastCarBlock = carBlockVector.back();
               connect(lastCarBlock,SIGNAL(statusChanged()),this,SLOT(updateView()),Qt::QueuedConnection);
               connect(lastCarBlock,SIGNAL(inProgress()),timer,SLOT(stop()),Qt::DirectConnection);
               connect(lastCarBlock,&CarBlock::progressFinished,[=](){timer->start(UPDATE_TIME);});
               connect(lastCarBlock,SIGNAL(changeStatusBar(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
           }
        }

        scrollWidget = new QWidget(ui->scrollArea);
        scrollLayout = new QVBoxLayout(scrollWidget);
        for(auto pos= carBlockVector.begin();pos!=carBlockVector.end();++pos)
            scrollLayout->addWidget(*pos);
        ui->scrollArea->setWidget(scrollWidget);
        ui->scrollArea->verticalScrollBar()->setValue(varticalPosition);
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Utracono połączenie z bazą danych!");
        ui->statusBar->showMessage("Nie można połączyć z bazą danych");
    }
}

void MainWindow::createUpdateButton()
{
    QPushButton * updateButton = new QPushButton(this);
    updateButton->setIcon(QIcon(":/images/images/update.png"));
    updateButton->setToolTip("Aktualizuj");
    updateButton->setStyleSheet("border:none;");
    ui->statusBar->addPermanentWidget(updateButton);
    connect(updateButton, &QPushButton::clicked,[=](){updateView();});
}

void MainWindow::on_pushButton_clicked()
{
  qDebug() << isConnectedToNetwork();
}
