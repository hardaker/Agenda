#include <QDebug>

#include "AgendaTopic.h"

AgendaTopic::AgendaTopic(QObject *parent) :
    QObject(parent)
{
}

AgendaTopic::AgendaTopic(const QString &topicName, const QTime &timeNeeded, const QTime &timeSpent, QObject *parent)
    : m_topicName(topicName), m_timeNeeded(timeNeeded), m_timeSpent(timeSpent)
{
}

const QString &AgendaTopic::topicName() const {
    return m_topicName;
}

const QTime &AgendaTopic::timeNeeded() const {
    return m_timeNeeded;
}

const QTime &AgendaTopic::timeSpent() const {
    return m_timeSpent;
}

QString AgendaTopic::formatTime(const QTime &aTime) const {
    if (aTime >= QTime(1,0))
        return aTime.toString("h'h':mm");
    return aTime.toString("mm:ss");
}

QString AgendaTopic::timeSpentStr() const {
    return formatTime(m_timeSpent);
}

QString AgendaTopic::timeNeededStr() const {
    return formatTime(m_timeNeeded);
}

void AgendaTopic::addTime(int thisManySeconds) {
    m_timeSpent = m_timeSpent.addSecs(thisManySeconds);
}

void AgendaTopic::addNeededTime(int thisManySeconds) {
    m_timeNeeded = m_timeNeeded.addSecs(thisManySeconds);
}
