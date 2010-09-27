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

QString AgendaTopic::timeSpentStr() const {
    return m_timeSpent.toString("mm:ss");
}

QString AgendaTopic::timeNeededStr() const {
    return m_timeNeeded.toString("mm:ss");
}

void AgendaTopic::addTime(int thisManySeconds) {
    m_timeSpent = m_timeSpent.addSecs(thisManySeconds);
}

void AgendaTopic::addNeededTime(int thisManySeconds) {
    m_timeNeeded = m_timeNeeded.addSecs(thisManySeconds);
}
