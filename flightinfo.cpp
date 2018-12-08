#include "flightinfo.h"
#include "ui_flightinfo.h"
#include "bookonflightdialog.h"
#include <QDebug>

FlightInfo::FlightInfo(QWidget *parent, FlightBooking *flight) :
    QDialog(parent),
    ui(new Ui::FlightInfo)
{
    ui->setupUi(this);
    this->setWindowTitle("Flight " + QString::number(flight->getId()));
    this->flight = flight;
    this->reloadTable();
}

FlightInfo::~FlightInfo()
{
    delete ui;
}

void FlightInfo::addRow(string ssn, string name, int age, int seat)
{
    int rowPos = ui->personTable->rowCount();
    ui->personTable->insertRow(rowPos);
    ui->personTable->setItem(rowPos, 0, new QTableWidgetItem(QString::fromStdString(ssn)));
    ui->personTable->setItem(rowPos, 1, new QTableWidgetItem(QString::fromStdString(name)));
    ui->personTable->setItem(rowPos, 2, new QTableWidgetItem(QString::number(age)));
}

void FlightInfo::reloadTable()
{
    ui->personTable->setRowCount(0);
    for (int i = 0; i < flight->getReserved(); i++)
    {
        Person *person = personList->get(flight->getPassangerSSNs().at(i))->person;
        this->addRow(person->getSsn(), person->getName(), person->getAge(), 0);
    }
}

void FlightInfo::on_bookFlight_clicked()
{
    BookOnFlightDialog *dialog = new BookOnFlightDialog(nullptr, flight);
    dialog->exec();
    if (dialog->selectedPersonSSN != "") flight->addPassanger(dialog->selectedPersonSSN);
    this->reloadTable();

}

void FlightInfo::on_unbookButton_clicked()
{
    string ssn = ui->personTable->item(ui->personTable->selectedItems().first()->row(), 0)->text().toStdString();
    flight->getPassangerSSNs().removeAt(flight->getPassangerSSNs().indexOf(ssn));
}
