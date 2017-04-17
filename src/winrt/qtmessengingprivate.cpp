#include "qtmessengingprivate.h"
#include <QtCore/qfunctions_winrt.h>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include <wrl.h>
#include <Windows.h>
#include <windows.foundation.h>
#include <Windows.ApplicationModel.Chat.h>
using namespace ABI::Windows::ApplicationModel::Chat;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
QtMessengingPrivate::QtMessengingPrivate(QObject *parent) :
    QObject(parent)
{
    ComPtr<IChatMessageManagerStatic> chatManager;
	HRESULT hr;
    hr = GetActivationFactory(HStringReference(RuntimeClass_Windows_ApplicationModel_Chat_ChatMessageManager).Get(),&chatManager);

    ComPtr<IChatMessage2> chatMessage;
    //hr = RoActivateInstance(HString::MakeReference(RuntimeClass_Windows_ApplicationModel_Chat_ChatMessage).Get(),&chatMessage);
	//ComPtr<IAsyncAction> action;
	//IChatMessage *chatMesage2 = 0x0;
	//hr = chatManager->ShowComposeSmsMessageAsync(chatMessage, &action);
	//qDebug() << "Should show composer: " << hr;
	//GetActivationFactory()
	//chatMessage->
}
void QtMessengingPrivate::sendMessage(const QString &phoneNumber, const QString &message) {
	QDesktopServices::openUrl(QUrl(QString("sms:%1?body=%2").arg(phoneNumber).arg(message)));
}