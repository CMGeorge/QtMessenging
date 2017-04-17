#include "qtmessengingprivate.h"
//#include <QtGui/qpa/qplatformnativeinterface.h>

#include <QDesktopServices>
#include <QGuiApplication>
#include <QUrl>

#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#import <Messages/Messages.h>
#import <MobileCoreServices/MobileCoreServices.h>


@interface MessageComposerDelegate:NSObject <MFMessageComposeViewControllerDelegate>{
    QtMessengingPrivate *m_qtMessengingPrivate;
}
@end
@implementation MessageComposerDelegate

-(id) initWithMessageDelegate:(QtMessengingPrivate*)qtMessengingPrivate{
    self = [super init];
    if (self){
        m_qtMessengingPrivate = qtMessengingPrivate;
    }
    return self;
}

- (void)messageComposeViewController:(MFMessageComposeViewController *)controller didFinishWithResult:(MessageComposeResult) result
{
    switch (result) {
        case MessageComposeResultCancelled:
        {
        }
            break;
        case MessageComposeResultFailed:
        {
            qDebug()<<"Just Failed";
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error"
                                                                           message:@"Failed to send SMS!"
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {
                                                                      Q_UNUSED(action);
                                                                  }];
            [alert addAction:defaultAction];
            [controller presentViewController:alert animated:YES completion:nil];
            break;
        }
        case MessageComposeResultSent:
        {
            qDebug()<<"SMS sent";
            //[[SpinnerIndicator sharedInstance] showSpinnerViewWithTitle:@"Completing transaction" andDetail:@"Please wait ..."];
        }
            break;
            
        default:
            break;
    }
    [controller dismissViewControllerAnimated:YES completion:nil];
}


@end
QtMessengingPrivate::QtMessengingPrivate(QObject *parent) :
    QObject(parent),
    m_delegate([[MessageComposerDelegate alloc] initWithMessageDelegate:this])
{

}

void QtMessengingPrivate::sendMessage(const QString &phoneNumber, const QString &message) {


    UIApplication* app = [UIApplication sharedApplication];

    if (app.windows.count <= 0)
        return false;

    UIWindow* rootWindow = app.windows[0];
    static UIViewController* rootViewController = rootWindow.rootViewController;

    if(![MFMessageComposeViewController canSendText]) {
        qDebug()<<"Can't send SMS with this configuration";
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error"
                                                                       message:@"Your device doesn't support SMS!"
                                                                preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleDefault
                                                              handler:^(UIAlertAction * action) {}];
        [alert addAction:defaultAction];
        [rootViewController presentViewController:alert animated:YES completion:nil];
        return;
    }
    MFMessageComposeViewController *messageController = [[MFMessageComposeViewController alloc] init];
    //NSArray *recipents = @[@200];// @[@kSendOrder];
    [messageController setMessageComposeDelegate:id(m_delegate)];
    messageController.recipients = @[phoneNumber.toNSString()];//recipents;
    messageController.body = @"This is a message";// message;
    qDebug()<<"Push the controller";
    [rootViewController presentViewController:messageController animated:YES completion:nil];
}
