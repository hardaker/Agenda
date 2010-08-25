#include "AgendaDialog.h"
#include "ui_AgendaDialog.h"

AgendaDialog::AgendaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgendaDialog)
{
    ui->setupUi(this);
}

AgendaDialog::~AgendaDialog()
{
    delete ui;
}
