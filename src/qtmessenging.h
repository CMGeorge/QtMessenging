#ifndef QTMESSENGING_H
#define QTMESSENGING_H

#include <QObject>
#include "qtmessengingprivate.h"
class QtMessenging : public QObject
{
    Q_OBJECT
public:
    explicit QtMessenging(QObject *parent = 0);
    Q_INVOKABLE void sendMessage(const QString &phoneNumber,const QString &message);
private:
    QtMessengingPrivate *m_qtMessengingPrivate;

signals:

public slots:
};

#endif // QTMESSENGING_H
