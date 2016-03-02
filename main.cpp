#include "mainwindow.h"
#include "dbconfigdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString line{};
    if(!DBConfigDialog::readFromFile(line))
        return 0;

    if(!line.isEmpty()){
        const QStringList parameters = line.split(";");
        Database::setParameters(parameters.at(0), parameters.at(1).toInt(), parameters.at(2), parameters.at(3), parameters.at(4));

        // check connection
        if(!Database::connectToDatabase())
            goto runCreator;
    }

    if(line.isEmpty()){
        runCreator:
        DBConfigDialog dbConfig(line);
        if(dbConfig.exec()==DBConfigDialog::Rejected)
            return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
