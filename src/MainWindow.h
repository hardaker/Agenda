#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QList>
#include <QtCore/QTimer>

#if defined(Q_WS_MAEMO_5) || defined(MAEMO_CHANGES)
#define IS_MAEMO 1
#endif

#include <QtDBus/QDBusInterface>

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
    void addTimeToTopic(int hours, int minutes, int seconds);
    void setupMenus();
    void setupAgendaTopics(QStringList topics);

public slots:
    void switchToNextTopic();
    void switchToPreviousTopic();
    void switchToTopic(int number);
    void updateScreenTimers();
    void timeElapsed();
    void calculateTotalTimes();
    void editTopics();
    void loadTopics();
    void triggerAlarm();
    void clearAlarm();
    void setUseLED(bool value);
    void startOrStop();

    void add15m();
    void add5m();
    void add1m();
    void minus1m();
    void minus5m();
    void minus15m();

private:
    Ui::MainWindow          *ui;
    QList<AgendaTopic *>    m_topics;
    int                     m_currentTopic;
    QTimer                  m_timer;
    QTime                   m_totalNeededTime;
    QTime                   m_spentTime;
    QTime                   m_deltaTime;
    bool                    m_deltaIsNegative;
    bool                    m_belowAlarmTime;
    QDBusInterface          *m_dbusInterface;
    int                     m_alarmTime;
    bool                    m_useLED;
};

#endif // MAINWINDOW_H