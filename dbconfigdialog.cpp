#include "dbconfigdialog.h"
#include "ui_dbconfigdialog.h"

DBConfigDialog::DBConfigDialog(const QString &line,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBConfigDialog)
{
    ui->setupUi(this);
    ui->lblWait->setVisible(false);
    ui->lblLoad->setVisible(false);

    if(!line.isEmpty()) {
        QStringList parameters = line.split(";");
        ui->leUser->setText(parameters.at(3));
        ui->lePassword->setText(parameters.at(4));
        ui->leAddress->setText(parameters.at(0));
        ui->lePort->setText(parameters.at(1));
    }

    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

DBConfigDialog::~DBConfigDialog()
{
    delete ui;
}

void DBConfigDialog::on_runButton_clicked()
{  
    // show waiting labels
    this->setEnabled(false);
    ui->lblWait->setVisible(true);
    ui->lblLoad->setVisible(true);
    qApp->processEvents();

    if(dataIsEmpty())
        return;

    Database::purgeDatabase();
    Database::setParameters(ui->leAddress->text(), ui->lePort->text().toInt(),
                            "sigmacars", ui->leUser->text(),
                            ui->lePassword->text());

    if(!writeToFile(ui->leAddress->text(), ui->lePort->text().toInt(),
                   "sigmacars", ui->leUser->text(),ui->lePassword->text()))
        return;


    if(Database::connectToDatabase()) {
        QMessageBox::information(this,"Informacja", "Pomyślnie połączono z bazą danych.");
        emit connectedToDB();
        ui->lblWait->setVisible(false);
        ui->lblLoad->setVisible(false);
        this->setEnabled(true);
        this->accept();
    }
    else {
        QMessageBox::critical(this,"Błąd!", "Nie połączono z bazą danych!");
        emit changeStatusBar("Nie można połączyć z bazą danych");
        ui->lblWait->setVisible(false);
        ui->lblLoad->setVisible(false);
        this->setEnabled(true);
        return;
    }
}

bool DBConfigDialog::writeToFile(const QString &hostname, int port, const QString &database, const QString &user, const QString &password)
{
    QFile initFile( QDir::currentPath()+"/init.txt" );
    if(!initFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this,"Błąd!", "Nie można otworzyć pliku z konfiguracją bazy danych.");
        return false;
    }
    QTextStream out( &initFile );
    out << hostname << ";" << QString::number(port) << ";" << database << ";" << user << ";" << password;
    initFile.close();

    return true;
}

bool DBConfigDialog::dataIsEmpty()
{
    if(ui->leUser->text().isEmpty() | ui->lePassword->text().isEmpty() || ui->leAddress->text().isEmpty() || ui->lePort->text().isEmpty()) {
        QMessageBox::warning(this,"Uwaga!","Pole tekstowe nie zostało wypełnione.");
        return true;
    }

    return false;
}

bool DBConfigDialog::readFromFile(QString &line)
{
    QFile initFile(QDir::currentPath()+"/init.txt");
    if (!initFile.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox(QMessageBox::Critical, QString("Błąd!"), QString("<font face=""Calibri"" size=""3"" color=""gray"">Nie można otworzyć pliku konfiguracji bazy danych.</font>"));
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
        msgBox.exec();
        return false;
    }
    QTextStream in(&initFile);
    line = in.readLine();
    initFile.close();

    return true;
}
