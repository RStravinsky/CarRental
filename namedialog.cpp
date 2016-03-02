#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog),
    idCar(id)
{
    ui->setupUi(this);

    historyTable = new QSqlQueryModel(this);
    historyTable->setQuery("SELECT Name, Surname, Destination FROM history;");

    setCompleterLists();
    setCompleters();

    QRegExp rx("^\\w+$");
    ui->lineEditName->setValidator(new QRegExpValidator(rx, this));
    ui->lineEditSurname->setValidator(new QRegExpValidator(rx, this));
    connect(ui->pushButtonCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

NameDialog::~NameDialog()
{
    delete ui;
}

void NameDialog::getNameAndSurname(QString &name, QString &surname, QString &destination,QString &target)
{
    name = ui->lineEditName->text();
    surname = ui->lineEditSurname->text();
    destination = ui->lineEditDestination->text();
    target = ui->lineEditTarget->text();
}

void NameDialog::on_pushButtonConfirm_released()
{
    QString name = ui->lineEditName->text();
    QString surname = ui->lineEditSurname->text();
    QString destination = ui->lineEditDestination->text();
    QString target = ui->lineEditTarget->text();

    if(name.isEmpty() || surname.isEmpty() || destination.isEmpty() || target.isEmpty())
        QMessageBox::warning(this,"Uwaga!","Pole tekstowe nie zostało wypełnione.");

    else this->accept();
}

void NameDialog::setCompleterLists()
{
    for(int i=0; i < historyTable->rowCount() ;++i) {
        nameList << historyTable->index(i, 0).data().toString();
        surnameList << historyTable->index(i, 1).data().toString();
        destinationList << historyTable->index(i, 2).data().toString();
    }

    nameList.removeDuplicates();
    surnameList.removeDuplicates();
    destinationList.removeDuplicates();
}
void NameDialog::setCompleters()
{
    nameCompleter = new QCompleter(nameList, ui->lineEditName);
    surnameCompleter = new QCompleter(surnameList, ui->lineEditSurname);
    destinationCompleter = new QCompleter(destinationList, ui->lineEditDestination);

    nameCompleter->setCompletionMode(QCompleter::PopupCompletion);
    surnameCompleter->setCompletionMode(QCompleter::PopupCompletion);
    destinationCompleter->setCompletionMode(QCompleter::PopupCompletion);

    nameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    surnameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    destinationCompleter->setCaseSensitivity(Qt::CaseInsensitive);

    nameCompleter->popup()->setStyleSheet(scrollBarStylesheet());
    surnameCompleter->popup()->setStyleSheet(scrollBarStylesheet());
    destinationCompleter->popup()->setStyleSheet(scrollBarStylesheet());

    nameCompleter->setMaxVisibleItems(10);
    surnameCompleter->setMaxVisibleItems(10);
    destinationCompleter->setMaxVisibleItems(10);

    ui->lineEditName->setCompleter(nameCompleter);
    ui->lineEditSurname->setCompleter(surnameCompleter);
    ui->lineEditDestination->setCompleter(destinationCompleter);
}

QString NameDialog::scrollBarStylesheet()
{
    return QString(
                "QScrollBar:vertical {"
                  "border: none;"
                  "background: transparent;"
                  "width: 8px;"
                  "margin: 20px 0px 20px 0px;"
                "}"

                "QScrollBar::handle:vertical {"
                  "background: rgb(107,124,140);"
                  "min-height: 20px;"
                  "border-radius: 2px;"
                "}"

                "QScrollBar::add-line:vertical {"
                   "border: none;"
                   "background: none;"
                "}"

                "QScrollBar::sub-line:vertical {"
                   "border: none;"
                   "background: none;"
                "}"
                );
}

