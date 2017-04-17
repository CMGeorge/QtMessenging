#include "qtmessengingprivate.h"
#include <QtAndroid>
#include <QDebug>

QtMessengingPrivate::QtMessengingPrivate(QObject *parent) :
    QObject(parent),
    m_conversionId(-1),
    m_needNotifyMsg(false)
{
    m_smsClient = QAndroidJniObject("ro/wesell/messenging/QtMessengingSMS");
    m_smsClient.setStaticField<jlong>("ro/wesell/messenging/QtMessengingSMS",
                                      "qtObject",
                                      reinterpret_cast<long>(this));

    QAndroidJniObject::callStaticMethod<void>("ro/wesell/messenging/QtMessengingSMS",
                                              "setActivity",
                                              "(Landroid/app/Activity;)V",
                                              QtAndroid::androidActivity().object<jobject>());

}
int QtMessengingPrivate::conversionId() const
{
    return m_conversionId;
}

void QtMessengingPrivate::setConversionId(int id)
{
    if (m_conversionId == id)
        return ;

    m_conversionId = id;
    emit conversionIdChanged(m_conversionId);
}
void QtMessengingPrivate::sendMessage(int thereadId,const QString &phoneNumber,const QString &message){
//    QString _currYear(QDate::currentDate().toString(QLatin1String("yyyy-")));
    QAndroidJniObject _phoneNumber = QAndroidJniObject::fromString(phoneNumber);
    QAndroidJniObject _msgText = QAndroidJniObject::fromString(message);
    QAndroidJniObject _sendedMsg = m_smsClient.callObjectMethod("sendMessage",
                                                               "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
                                                               (jlong)thereadId,
                                                               _phoneNumber.object<jstring> (),
                                                               _msgText.object<jstring> () );
}

void QtMessengingPrivate::onMsgReceived(jlong threadid)
{
    qDebug() << Q_FUNC_INFO << ">" << threadid;
    m_needNotifyMsg = true;
    //emit reloadConversionList();
}
void QtMessengingPrivate::onMsgSendOut(jlong msgid, jboolean isSuccess)
{
    qDebug() << Q_FUNC_INFO << ">" << msgid << ":" << isSuccess;
}

