#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_topics.push_back(new AgendaTopic("topic #1", QTime(0,5)));
    m_topics.push_back(new AgendaTopic("topic #2", QTime(0,15)));

    m_currentTopic = 1;

    mainWindowSetup(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainWindowSetup(Ui::MainWindow *ui) {
    switchToTopic(m_currentTopic);

    ui->totalTime->setText("4:05 / 12:00");
    ui->timeLeft->setText("0:05 / 5:00");

}

void MainWindow::switchToTopic(int number) {
    if (number > m_topics.count() || number < 1)
        return;

    AgendaTopic *topic = m_topics[number-1];
    ui->topic->setText(topic->topicName());

    if (number < m_topics.count()) {
        ui->nextTopic->setText(m_topics[number]->topicName());
    } else {
        ui->nextTopic->setText("");
    }
}

void MainWindow::switchToNextTopic() {
    if (m_currentTopic < m_topics.count())
        switchToTopic(++m_currentTopic);
}

void MainWindow::switchToPreviousTopic() {
    if (m_currentTopic > 1)
        switchToTopic(--m_currentTopic);
}
