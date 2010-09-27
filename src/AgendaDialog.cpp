#include "AgendaDialog.h"
#include "ui_AgendaDialog.h"
#include <QDebug>

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

QStringList AgendaDialog::getTopics() {
    QString topicText = ui->agendaText->document()->toPlainText();
    QStringList topics = topicText.split('\n', QString::SkipEmptyParts);
    return topics;
}

void AgendaDialog::setTopics(const QString &topics) {
    ui->agendaText->setText(topics);
}
