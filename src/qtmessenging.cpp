#include "qtmessenging.h"
#include <QDebug>
QtMessenging::QtMessenging(QObject *parent) : QObject(parent)
{
    m_qtMessengingPrivate = new QtMessengingPrivate(/*this*/);
}

void QtMessenging::sendMessage(const QString &phoneNumber,const QString &message){
#ifdef Q_OS_ANDROID
    m_qtMessengingPrivate->sendMessage(0,phoneNumber,message);
#else
    qDebug()<<"Please ipmplement";
	m_qtMessengingPrivate->sendMessage(phoneNumber, message);
#endif
}
