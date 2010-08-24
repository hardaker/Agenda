#ifndef AGENDATOPIC_H
#define AGENDATOPIC_H

#include <QObject>
#include <QtCore/QString>
#include <QtCore/QTime>

class AgendaTopic : public QObject
{
    Q_OBJECT
public:
    explicit AgendaTopic(QObject *parent = 0);
    AgendaTopic(const QString &topicName, const QTime &timeNeeded, const QTime &timeSpent = QTime(0,0), QObject *parent = 0);

    const QString &topicName() const;
    const QTime   &timeNeeded() const;
    const QTime   &timeSpent() const;
    const QString &timeSpentStr() const;
    void addTime(int thisManySeconds);

signals:

public slots:

private:
    QString     m_topicName;
    QTime       m_timeNeeded;
    QTime       m_timeSpent;
};

#endif // AGENDATOPIC_H
