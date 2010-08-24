#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QList>

#include "AgendaTopic.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mainWindowSetup(Ui::MainWindow *ui);

public slots:
    void switchToNextTopic();
    void switchToPreviousTopic();
    void switchToTopic(int number);

private:
    Ui::MainWindow          *ui;
    QList<AgendaTopic *>    m_topics;
    int                     m_currentTopic;
};

#endif // MAINWINDOW_H
