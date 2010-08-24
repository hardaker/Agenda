#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowSetup(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainWindowSetup(Ui::MainWindow *ui) {
    ui->nextTopic->setText("foo");
    ui->topic->setText("here");

    ui->totalTime->setText("4:05 / 12:00");
    ui->timeLeft->setText("0:05 / 5:00");

}
