#ifndef QTMESSENGINGPRIVATE_H
#define QTMESSENGINGPRIVATE_H

#include <QObject>

#include <QAndroidJniObject>
#include <jni.h>

void QtSmsClient_jniOnMsgReceived(JNIEnv *, jobject, jlong, jlong);
void QtSmsClient_jniOnMsgSendOut(JNIEnv *, jobject, jlong, jlong, jboolean);
void QtSmsClient_jniOnMsgDelivered(JNIEnv *, jobject, jlong, jlong);
void QtSmsClient_jniOnMsgConversionCreated(JNIEnv *, jobject, jlong, jlong);


class QtMessengingPrivate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int conversionId READ conversionId WRITE setConversionId NOTIFY conversionIdChanged)
public:
    explicit QtMessengingPrivate(QObject *parent = 0);
    Q_INVOKABLE int conversionId() const;
    void sendMessage(int thereadId,const QString &phoneNumber,const QString &message);
private:
    QAndroidJniObject m_smsClient;
    int m_conversionId;
    bool m_needNotifyMsg;

signals:
    void conversionIdChanged(int id);
    void smsSent(bool status);
    void smsReceived();
public slots:
    void setConversionId(int id);

protected:
    friend void QtSmsClient_jniOnMsgReceived(JNIEnv *, jobject, jlong, jlong);
    friend void QtSmsClient_jniOnMsgSendOut(JNIEnv *, jobject, jlong, jlong, jboolean);
    void onMsgReceived(jlong threadid);
    void onMsgSendOut(jlong msgid, jboolean isSuccess);
};

#endif // QTMESSENGINGPRIVATE_H
