#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_timer(), m_totalNeededTime(0,0), m_spentTime(0,0)
{
    ui->setupUi(this);

    m_topics.push_back(new AgendaTopic("topic #1", QTime(0,5)));
    m_topics.push_back(new AgendaTopic("topic #2", QTime(0,15)));

    calculateTotalTimes();

    m_currentTopic = 1;

    mainWindowSetup(ui);

    connect(ui->next, SIGNAL(clicked()), this, SLOT(switchToNextTopic()));
    connect(ui->previous, SIGNAL(clicked()), this, SLOT(switchToPreviousTopic()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(startOrStop()));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeElapsed()));
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
    ui->timeLeft->setText(m_topics[m_currentTopic-1]->timeSpentStr() + QString(" / ") + m_topics[m_currentTopic-1]->timeNeededStr());
    ui->totalTime->setText(m_spentTime.toString("mm:ss") + QString(" / ") + m_totalNeededTime.toString("mm:ss"));
}

void MainWindow::timeElapsed() {
    m_topics[m_currentTopic-1]->addTime(1);
    m_spentTime = m_spentTime.addSecs(1);
    updateScreenTimers();
}

void MainWindow::startOrStop() {
    if (m_timer.isActive()) {
        m_timer.stop();
        ui->start->setText("Start");
    } else {
        m_timer.start(1000);
        ui->start->setText("Pause");
    }
}

void MainWindow::calculateTotalTimes() {
    QList<AgendaTopic *>::iterator begin;
    QList<AgendaTopic *>::iterator end = m_topics.end();

    m_totalNeededTime = QTime(0,0);
    m_spentTime = QTime(0,0);

    for(begin = m_topics.begin(); begin != end; begin++) {
        QTime newTime = (*begin)->timeNeeded();
        m_totalNeededTime = m_totalNeededTime.addSecs(newTime.second() + 60 * newTime.minute() + 3600 * newTime.hour());
        newTime = (*begin)->timeSpent();
        m_spentTime = m_spentTime.addSecs(newTime.second() + 60 * newTime.minute() + 3600 * newTime.hour());
    }
}
