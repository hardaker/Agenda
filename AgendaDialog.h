#ifndef AGENDADIALOG_H
#define AGENDADIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class AgendaDialog;
}

class AgendaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AgendaDialog(QWidget *parent = 0);
    ~AgendaDialog();

private:
    Ui::AgendaDialog *ui;
};

#endif // AGENDADIALOG_H
