#include "bookingdialog.h"
#include "ui_bookingdialog.h"

BookingDialog::BookingDialog(QSqlQueryModel *bookTable, int id, QWidget *parent) :
    bookingTable(bookTable),
    idCar(id),
    QDialog(parent),
    ui(new Ui::BookingDialog)
{
    ui->setupUi(this);
    setCalendarColor(ui->calendarWidget,QColor(255,140,0));

    bookingBlockVector.emplace_back(std::move(new BookingBlock("Rafał Strawiński", "Status", QTime::currentTime(),QTime::currentTime())));
    bookingBlockVector.emplace_back(std::move(new BookingBlock("Bartłomiej Pokrzywa", "Status", QTime::currentTime(),QTime::currentTime())));
    bookingBlockVector.emplace_back(std::move(new BookingBlock("Emil Wiśniewski", "Status", QTime::currentTime(),QTime::currentTime())));
    bookingBlockVector.emplace_back(std::move(new BookingBlock("Michał Wodziński", "Status", QTime::currentTime(),QTime::currentTime())));
    bookingBlockVector.emplace_back(std::move(new BookingBlock("Michał Stojek", "Status", QTime::currentTime(),QTime::currentTime())));
    bookingBlockVector.emplace_back(std::move(new BookingBlock("Rafał Strawiński", "Status", QTime::currentTime(),QTime::currentTime())));
    bookingBlockVector.emplace_back(std::move(new BookingBlock("Bartłomiej Pokrzywa", "Status", QTime::currentTime(),QTime::currentTime())));

    scrollWidget = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);
    for(auto pos= bookingBlockVector.begin();pos!=bookingBlockVector.end();++pos)
        scrollLayout->addWidget(*pos);
    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->setWidgetResizable(true);

    fillCalendar();
}

BookingDialog::~BookingDialog()
{
    delete ui;
}

void BookingDialog::dateClicked(QDate date)
{
    choosenDate = date;
}

void BookingDialog::fillCalendar()
{
    QTextCharFormat format;
    //format.setForeground(QBrush(Qt::magenta, Qt::SolidPattern));
    format.setBackground(QBrush(Qt::cyan, Qt::SolidPattern));

    for(int i = 0; i < bookingTable->rowCount(); ++i) {

        if(bookingTable->data(bookingTable->index(i,5)).toInt() == idCar) {

            QDate itDate = bookingTable->data(bookingTable->index(i,3)).toDate();

            while(itDate <= bookingTable->data(bookingTable->index(i,4)).toDate()) {
                ui->calendarWidget->setDateTextFormat(itDate, format);
                itDate = itDate.addDays(1);
            }

        }

    }

}


void BookingDialog::setCalendarColor(QCalendarWidget *&calendarWidget,QColor color)
{
    QWidget *calendarNavBar = calendarWidget->findChild<QWidget *>("qt_calendar_navigationbar");
    if (calendarNavBar) {
        QPalette pal = calendarNavBar->palette();
        pal.setColor(calendarNavBar->backgroundRole(), color);
        calendarNavBar->setPalette(pal);
    }
}