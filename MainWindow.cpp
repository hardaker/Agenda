#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AgendaDialog.h"

#include <QtGui/QMenuBar>
#include <QtGui/QActionGroup>
#include <QtGui/QAction>
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
    setupMenus();

    connect(ui->next, SIGNAL(clicked()), this, SLOT(switchToNextTopic()));
    connect(ui->previous, SIGNAL(clicked()), this, SLOT(switchToPreviousTopic()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(startOrStop()));

    connect(ui->plus15, SIGNAL(clicked()), this, SLOT(add15m()));
    connect(ui->plus5, SIGNAL(clicked()), this, SLOT(add5m()));
    connect(ui->plus1, SIGNAL(clicked()), this, SLOT(add1m()));

    connect(ui->minus15, SIGNAL(clicked()), this, SLOT(minus15m()));
    connect(ui->minus5, SIGNAL(clicked()), this, SLOT(minus5m()));
    connect(ui->minus1, SIGNAL(clicked()), this, SLOT(minus1m()));

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

void MainWindow::setupMenus() {
    QAction *action = menuBar()->addAction("Topics");
    connect(action, SIGNAL(triggered()), this, SLOT(editTopics()));
}

void MainWindow::editTopics() {
    AgendaDialog topicDialog;
    QString list;
    for(QList<AgendaTopic *>::iterator begin = m_topics.begin(); begin != m_topics.end(); begin++) {
        list = list + (*begin)->topicName() + QString("\n");
    }
    topicDialog.setTopics(list);
    int result = topicDialog.exec();
    if (result == QDialog::Accepted) {
        m_topics.clear(); // X mem leak

        QStringList topics = topicDialog.getTopics();
        QStringList::iterator begin;
        QStringList::iterator end = topics.end();
        for(begin = topics.begin(); begin != end; begin++) {
            m_topics.append(new AgendaTopic(*begin, QTime(0,0)));
        }
    }
    qDebug() << "done";
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
    QString timerText;
    if (m_topics[m_currentTopic-1]->timeSpent() > m_topics[m_currentTopic-1]->timeNeeded()) {
        timerText = "<font color=\"red\">";
    } else {
        timerText = "<font color=\"green\">";
    }
    ui->timeLeft->setText(timerText + m_topics[m_currentTopic-1]->timeSpentStr() + QString(" / ") + m_topics[m_currentTopic-1]->timeNeededStr() + "</font>");

    if (m_spentTime > m_totalNeededTime) {
        timerText = "<font color=\"red\">";
    } else {
        timerText = "<font color=\"green\">";
    }
    ui->totalTime->setText(timerText + m_spentTime.toString("mm:ss") + QString(" / ") + m_totalNeededTime.toString("mm:ss") + "</font>");
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

void MainWindow::addTimeToTopic(int hours, int minutes, int seconds) {
    m_topics[m_currentTopic-1]->addNeededTime(hours * 3600 + minutes * 60 + seconds);
    calculateTotalTimes();
    updateScreenTimers();
}

void MainWindow::add15m() {
    addTimeToTopic(0,15,0);
}

void MainWindow::add5m() {
    addTimeToTopic(0,5,0);
}

void MainWindow::add1m() {
    addTimeToTopic(0,1,0);
}

void MainWindow::minus15m() {
    addTimeToTopic(0,-15,0);
}

void MainWindow::minus5m() {
    addTimeToTopic(0,-5,0);
}

void MainWindow::minus1m() {
    addTimeToTopic(0,-1,0);
}




