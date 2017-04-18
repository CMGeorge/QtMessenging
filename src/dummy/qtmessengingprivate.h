#ifndef QTMESSENGINGPRIVATE_H
#define QTMESSENGINGPRIVATE_H

#include <QObject>

class QtMessengingPrivate : public QObject
{
    Q_OBJECT
public:
    explicit QtMessengingPrivate(QObject *parent = 0);
    void sendMessage(const QString &phoneNumber, const QString &message);

signals:

public slots:
};

#endif // QTMESSENGINGPRIVATE_H
