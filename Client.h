#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui>
#include <QtNetwork>
#include "ui_client.h"
#include "user.h"
#include <QMainWindow>
#include "serverDialog.h"
#include "usernameDialog.h"
#include <QString>
#include "infoDialog.h"
#include <QColorDialog>
#include "messagesColor.h"
#include <QFileDialog>
#include "userInfoDialog.h"
#include <QtMultimedia/QMediaPlayer>
#include "customStatusDialog.h"
#include "fileSendDialog.h"
#include "imageFullDialog.h"
#include <QDomDocument>
#include <QFontDialog>

class Client : public QMainWindow, private Ui::Client {

    Q_OBJECT

    public:
        Client();
        void setBackColor(QColor);
        QColor getBackColor();
        QString username="Test";
        QString status="Занят";
        QString autoFileGet="no";
        QImage icon;
        QString icon_64;
        void toggleDateTimeFlag();
        bool getDateTimeFlag();
        void toggleIpFlag();
        bool getIpFlag();
        void setMessageColor(QColor);
        QColor getMessageColor();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        void updateUserList();
        void remove(QLayout *layout);
        void updatedUser();
        QString sizeConvert(int sizeBytes);
        void countPercentSend();
        void countPercentGet();
        void on_exitAction_triggered();
        void readSettings();
        void saveXMLLogs();
        QDomElement log(QDomDocument *domDoc, const QString &dateTime, const QString &ip, const QString &name, const QString &type, const QString &message);
        QDomElement makeElement(QDomDocument *domDoc, const QString &strName, const QString &stdAttr, const QString &Text);

    private slots:
        void on_sendPhoto_clicked();
        void on_connectButton_clicked();
        void on_sendButton_clicked();
        void on_message_returnPressed();
        void dataReceived();
        void connectUser();
        void logOut();
        void socketError(QAbstractSocket::SocketError error);
        void on_accept();
        void on_serverButton_clicked();
        void on_usernameButton_clicked();
        void on_acceptUsername();
        void on_infoButton_clicked();
        void on_dateTimeButton_clicked();
        void on_ipButton_clicked();
        void on_actionBackColor_triggered();
        void on_actionMessagesColor_triggered();
        void on_acceptMessagesColor();
        void on_statusBarButton_clicked();
        void preLogOut();
        void on_photoButton_clicked();
        void on_statusAvailable();
        void on_statusAfk();
        void on_statusNotDisturb();
        void on_statusCustom();
        void on_statusCustomOpen();
        void on_showContextMenuUsers(const QPoint&);
        void openInfoUser();
        void openFileSending();
        void on_sendFile();
        void on_showContextMenuMessagesListUsers(const QPoint&);
        void openFullSizeImage();
        void saveImage();
        void on_saveLogs();
        void on_colorChangeButton_clicked();
        void on_fontChangeButton_clicked();
        void on_formatSendButton_clicked();
        void on_autoFileGetButton_clicked();
    private:
        QAction *sAction;
        QAction *sendButton;
        QAction *sendPhoto;
        QAction *sendFormat;
        bool formatSending=false;
        QString id;
        QTcpSocket *socket;
        quint16 messageLength;
        User *user;
        QHostAddress ip=QHostAddress("127.0.0.1");
        quint16 port=45678;
        ServerDialog *sDialog;
        UsernameDialog *uDialog;
        InfoDialog *infoDialog;
        CustomStatusDialog *statusDialog;
        MessagesDialog *mDialog;
        bool dateTimeFlag=false;
        bool ipFlag=false;
        QColor color=Qt::white;
        QColor messageColor=Qt::black;
        QColor infoUsersColor=Qt::black;
        QList<User *> userList;
        QMap<QByteArray, QString> iconsList;
        FileSendDialog *fDialog;
        QLabel *amountGettingFiles;
        QLabel *fileGettingProgress;
        QLabel *fileGettingSize;
        QLabel *amountSendingFiles;
        QLabel *fileSendingProgress;
        QLabel *fileSendingSize;
        QHBoxLayout *gettingLayout;
        QHBoxLayout *sendingLayout;
        QVBoxLayout *statusBarLayout;
        int amountSendFiles=0;
        int amountGetFiles=0;
        int amountSendedFiles=0;
        int amountGettedFiles=0;
        int percentProgressSend=0;
        int percentProgressGet=0;
        int sizeBytesSend=0;
        int sizeBytesGet=0;
        QList<FileSendDialog *>fileSendList;
        int openedAmount=0;
        QDomDocument *doc;
        QDomElement domElement;
        QMap<int, QImage> photosList;
        int lastPos;
        ImageFullDialog *iDialog;
        QColor formattedColor=Qt::black;
        QFont formattedFont;
};

#endif // CLIENT_H
