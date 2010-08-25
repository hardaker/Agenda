#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_timer()
{
    ui->setupUi(this);

    m_topics.push_back(new AgendaTopic("topic #1", QTime(0,5)));
    m_topics.push_back(new AgendaTopic("topic #2", QTime(0,15)));

    m_currentTopic = 1;

    mainWindowSetup(ui);

    connect(ui->next, SIGNAL(clicked()), this, SLOT(switchToNextTopic()));
    connect(ui->previous, SIGNAL(clicked()), this, SLOT(switchToPreviousTopic()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->pause, SIGNAL(clicked()), this, SLOT(pause()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainWindowSetup(Ui::MainWindow *ui) {
    switchToTopic(m_currentTopic);
    updateScreenTimers();
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
    updateScreenTimers();
}

void MainWindow::switchToNextTopic() {
    if (m_currentTopic < m_topics.count())
        switchToTopic(++m_currentTopic);
}

void MainWindow::switchToPreviousTopic() {
    if (m_currentTopic > 1)
        switchToTopic(--m_currentTopic);
}

void MainWindow::updateScreenTimers() {
    qDebug() << "setting to" << m_topics[m_currentTopic-1]->timeSpentStr();
    ui->timeLeft->setText(m_topics[m_currentTopic-1]->timeSpentStr());
}

void MainWindow::timeElapsed() {
    qDebug() << "elapsed";
    m_topics[m_currentTopic-1]->addTime(1);
    qDebug() << "new time:" << m_topics[m_currentTopic-1]->timeSpentStr();
    updateScreenTimers();
}

void MainWindow::pause() {
    m_timer.stop();
}

void MainWindow::start() {
    qDebug() << "starting";
    m_timer.start(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeElapsed()));
}
