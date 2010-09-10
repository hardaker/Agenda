#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AgendaDialog.h"

#include <QtGui/QMenuBar>
#include <QtGui/QActionGroup>
#include <QtGui/QAction>
#include <QDebug>
#if IS_MAEMO
#include <mce/dbus-names.h>
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_timer(), m_totalNeededTime(0,0), m_spentTime(0,0), m_belowAlarmTime(false),
    m_dbusInterface(0), m_alarmTime(60)
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

#ifdef IS_MAEMO
    m_dbusInterface = new QDBusInterface(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF, QDBusConnection::systemBus(), this);
    QDBusMessage reply = m_dbusInterface->call(MCE_ENABLE_LED);
    if (reply.type() == QDBusMessage::ErrorMessage)
        qDebug() << reply.errorMessage();
#endif
}

MainWindow::~MainWindow()
{
    clearAlarm();
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

    // generate the current list of topics into a string
    QString list;
    for(QList<AgendaTopic *>::iterator begin = m_topics.begin(); begin != m_topics.end(); begin++) {
        list = list + (*begin)->timeNeededStr() + "\t" + (*begin)->topicName() + QString("\n");
    }
    topicDialog.setTopics(list);

    // exec the dialog for editng
    int result = topicDialog.exec();

    // parse the results
    if (result == QDialog::Accepted) {
        m_topics.clear(); // X mem leak

        QRegExp exp("^\\s*(\\d+):(\\d+)\\s+(.*)");
        QStringList results;

        QStringList topics = topicDialog.getTopics();
        QStringList::iterator begin;
        QStringList::iterator end = topics.end();
        for(begin = topics.begin(); begin != end; begin++) {
            if(exp.indexIn(*begin) != -1) {
                m_topics.append(new AgendaTopic(exp.cap(3),
                                                QTime(0, exp.cap(1).toInt(), exp.cap(2).toInt())));
            } else {
                m_topics.append(new AgendaTopic(*begin, QTime(0,0)));
            }
        }
    }
    m_currentTopic = 1;
    calculateTotalTimes();
    switchToTopic(m_currentTopic);
    qDebug() << "done";
}

void MainWindow::switchToTopic(int number) {
    if (number > m_topics.count() || number < 1)
        return;

    m_currentTopic = number;
    AgendaTopic *topic = m_topics[number-1];
    ui->topic->setText(topic->topicName());

    if (number < m_topics.count()) {
        ui->nextTopic->setText(m_topics[number]->topicName());
    } else {
        ui->nextTopic->setText("");
    }

    clearAlarm();
    calculateTotalTimes();
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

    // Display the current agenda's time left
    if (m_topics[m_currentTopic-1]->timeSpent() > m_topics[m_currentTopic-1]->timeNeeded()) {
        timerText = "<font color=\"red\">";
    } else {
        timerText = "<font color=\"green\">";
    }
    ui->timeLeft->setText(timerText + m_topics[m_currentTopic-1]->timeSpentStr() + QString(" / ") + m_topics[m_currentTopic-1]->timeNeededStr() + "</font>");

    // Maybe trigger the alarm
    QTime curTime = m_topics[m_currentTopic-1]->timeSpent().addSecs(m_alarmTime);
    qDebug() << "checking: " << m_belowAlarmTime << " - " << curTime << " - " <<  m_topics[m_currentTopic-1]->timeNeeded();
    if (!m_belowAlarmTime && curTime > m_topics[m_currentTopic-1]->timeNeeded()) {
        triggerAlarm();
    }

    // Display th e total quantity of time left
    if (m_spentTime > m_totalNeededTime) {
        timerText = "<font color=\"red\">";
    } else {
        timerText = "<font color=\"green\">";
    }
    ui->totalTime->setText(timerText + m_spentTime.toString("mm:ss") + QString(" / ") + m_totalNeededTime.toString("mm:ss") + "</font>");

    if (m_deltaIsNegative) {
        timerText = "<font color=\"red\"> -";
    } else {
        timerText = "<font color=\"green\"> +";
    }
    ui->delta->setText(timerText + m_deltaTime.toString("mm:ss") + "</font>");
}

void MainWindow::triggerAlarm() {
    clearAlarm();
    m_belowAlarmTime = true;

    qDebug() << "alarm time";
#ifdef IS_MAEMO
    QDBusMessage reply;

    // set the LED pattern
    reply = m_dbusInterface->call(MCE_ACTIVATE_LED_PATTERN, "PatternError");
    if (reply.type() == QDBusMessage::ErrorMessage)
        qDebug() << reply.errorMessage();

    // turn on the display too
    reply = m_dbusInterface->call(MCE_DISPLAY_ON_REQ);
    if (reply.type() == QDBusMessage::ErrorMessage)
        qDebug() << reply.errorMessage();
#endif
}

void MainWindow::clearAlarm() {
    if (!m_belowAlarmTime)
        return;

    qDebug() << "clearing time";

    m_belowAlarmTime = false;
#ifdef IS_MAEMO
    QDBusMessage reply;
    reply = m_dbusInterface->call(MCE_DEACTIVATE_LED_PATTERN, "PatternError");
    if (reply.type() == QDBusMessage::ErrorMessage)
        qDebug() << reply.errorMessage();
#endif
}

void MainWindow::timeElapsed() {
    m_topics[m_currentTopic-1]->addTime(1);
    m_spentTime = m_spentTime.addSecs(1);
    if (m_deltaIsNegative)
        m_deltaTime = m_deltaTime.addSecs(1);
    else if (m_deltaTime == QTime(0,0)) {
        m_deltaIsNegative = true;
        m_deltaTime = QTime(0,0,1);
    } else {
        m_deltaTime = m_deltaTime.addSecs(-1);
    }
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
    QTime timeTillNow = QTime(0,0); // amount in schedule until now

    m_totalNeededTime = QTime(0,0);
    m_spentTime = QTime(0,0);
    m_deltaIsNegative = false;
    m_deltaTime = QTime(0,0);

    int count = 0;
    for(begin = m_topics.begin(); begin != end; begin++) {
        QTime newTime = (*begin)->timeNeeded();
        QTime neededTime = (*begin)->timeNeeded();

        m_totalNeededTime = m_totalNeededTime.addSecs(newTime.second() + 60 * newTime.minute() + 3600 * newTime.hour());
        newTime = (*begin)->timeSpent();
        m_spentTime = m_spentTime.addSecs(newTime.second() + 60 * newTime.minute() + 3600 * newTime.hour());

        if (m_currentTopic > count) {
            timeTillNow = m_spentTime;
            m_deltaTime = m_totalNeededTime;
        }
        count++;
    }

    qDebug() << "-----";
    qDebug() << "time till now: " << timeTillNow;
    qDebug() << "needed before this point: " << m_deltaTime;
    qDebug() << "spent time: " << m_spentTime;

    if (m_spentTime <= m_deltaTime) {
        // yay, we're ahead of schedule
        m_deltaIsNegative = false;
        int secondsToAdd = - (m_spentTime.second() + 60 * m_spentTime.minute() + 3600 * m_spentTime.hour());
        qDebug() << "adding:" << secondsToAdd;
        m_deltaTime = m_deltaTime.addSecs(secondsToAdd);
    } else {
        m_deltaIsNegative = true;
        QTime tmpTime;
        tmpTime = m_deltaTime;
        m_deltaTime = m_spentTime;
        m_deltaTime = m_deltaTime.addSecs(- (tmpTime.second() + 60 * tmpTime.minute() + 3600 * tmpTime.hour()));
    }
    qDebug() << "result: " << m_deltaTime;
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




