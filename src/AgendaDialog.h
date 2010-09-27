#ifndef AGENDADIALOG_H
#define AGENDADIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QStringList>

namespace Ui {
    class AgendaDialog;
}

class AgendaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AgendaDialog(QWidget *parent = 0);
    ~AgendaDialog();

    QStringList getTopics();
    void setTopics(const QString &topics);

private:
    Ui::AgendaDialog *ui;
};

#endif // AGENDADIALOG_H
