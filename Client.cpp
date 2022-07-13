#include "Client.h"
#include <QSound>

Client::Client() {
    setupUi(this);

    horizontalLayout->setAlignment(Qt::AlignHCenter);
    setWindowTitle("Клиент");
    QActionGroup *groupStatuses=new QActionGroup(this);
    groupStatuses->addAction(availableAction);
    groupStatuses->addAction(afkAction);
    groupStatuses->addAction(notDisturbAction);
    groupStatuses->addAction(customAction);
    groupStatuses->setExclusive(true);
    messagesList->setStyleSheet("background-color: ");
    infoDialog=new InfoDialog(this);
    sDialog=new ServerDialog(this);
    uDialog=new UsernameDialog(this);
    mDialog=new MessagesDialog(this);
    statusDialog=new CustomStatusDialog(this);
    iDialog=new ImageFullDialog(this);
    sAction=serverAction;
    sAction->setText("Сервер: "+ip.toString()+":"+QString::number(port));
    socket=new QTcpSocket(this);
    connect(messagesColorAction, SIGNAL(triggered()), this, SLOT(on_actionMessagesColor_triggered()));
    connect(backColorAction, SIGNAL(triggered()), this, SLOT(on_actionBackColor_triggered()));
    connect(ipAction, SIGNAL(triggered()), this, SLOT(on_ipButton_clicked()));
    connect(dateTimeAction, SIGNAL(triggered()), this, SLOT(on_dateTimeButton_clicked()));
    connect(infoAction, SIGNAL(triggered()), this, SLOT(on_infoButton_clicked()));
    connect(usernameAction, SIGNAL(triggered()), this, SLOT(on_usernameButton_clicked()));
    connect(connectAction, SIGNAL(triggered()), this, SLOT(on_connectButton_clicked()));
    connect(sAction, SIGNAL(triggered()), this, SLOT(on_serverButton_clicked()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(socket, SIGNAL(connected()), this, SLOT(connectUser()));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(preLogOut()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(logOut()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(statusBarAction, SIGNAL(triggered()), this, SLOT(on_statusBarButton_clicked()));
    connect(photoAction, SIGNAL(triggered()), this, SLOT(on_photoButton_clicked()));
    connect(availableAction, SIGNAL(triggered()), this, SLOT(on_statusAvailable()));
    connect(afkAction, SIGNAL(triggered()), this, SLOT(on_statusAfk()));
    connect(notDisturbAction, SIGNAL(triggered()), this, SLOT(on_statusNotDisturb()));
    connect(customAction, SIGNAL(triggered()), this, SLOT(on_statusCustomOpen()));
    connect(saveLogsAction, SIGNAL(triggered()), this, SLOT(on_saveLogs()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(on_exitAction_triggered()));
    connect(messageColorButton, SIGNAL(clicked()), this, SLOT(on_colorChangeButton_clicked()));
    connect(fontButton, SIGNAL(clicked()), this, SLOT(on_fontChangeButton_clicked()));
    connect(fileAutoAction, SIGNAL(triggered()), this, SLOT(on_autoFileGetButton_clicked()));

    messageLength = 0;
    sendButtonList->setPopupMode(QToolButton::MenuButtonPopup);
    sendButton=new QAction("Отправить сообщение");
    sendPhoto=new QAction("Отправить фото");
    sendFormat=new QAction("Формат. сообщение");
    sendButtonList->setDefaultAction(sendButton);
    sendButtonList->addAction(sendButton);
    sendButtonList->addAction(sendPhoto);
    sendButtonList->addAction(sendFormat);
    connect(sendButton, SIGNAL(triggered()), this, SLOT(on_sendButton_clicked()));
    connect(sendPhoto, SIGNAL(triggered()), this, SLOT(on_sendPhoto_clicked()));
    connect(sendFormat, SIGNAL(triggered()), this, SLOT(on_formatSendButton_clicked()));

    usersList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(usersList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_showContextMenuUsers(QPoint)));

    messagesList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(messagesList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_showContextMenuMessagesListUsers(QPoint)));

    fontButton->hide();
    messageColorButton->hide();

    amountGettingFiles=new QLabel("Кол-во получаемых файлов: 0");
    fileGettingProgress=new QLabel("Прогресс получения файлов: 0%");
    fileGettingSize=new QLabel("Размер получаемых файлов: 0 байт");

    amountSendingFiles=new QLabel("Кол-во передаваемых файлов: 0");
    fileSendingProgress=new QLabel("Прогресс отправки файлов: 0%");
    fileSendingSize=new QLabel("Размер отправляемых файлов: 0 байт");

    statusBar()->addWidget(amountGettingFiles);
    statusBar()->addWidget(fileGettingProgress);
    statusBar()->addWidget(fileGettingSize);
    statusBar()->addWidget(amountSendingFiles);
    statusBar()->addWidget(fileSendingProgress);
    statusBar()->addWidget(fileSendingSize);

    doc=new QDomDocument("logs");
    domElement=doc->createElement("logs");
    doc->appendChild(domElement);

    readSettings();

}

void Client::closeEvent(QCloseEvent *event) {
    event->ignore();
    on_exitAction_triggered();
}

void Client::readSettings() {
    QSettings settings("/home/user/Desktop/ChatClient.config", QSettings::IniFormat);
    setBackColor(qvariant_cast<QColor>(settings.value("background_message_color")));
    setMessageColor(qvariant_cast<QColor>(settings.value("message_color")));
    username=qvariant_cast<QString>(settings.value("username"));
    icon_64=qvariant_cast<QString>(settings.value("icon_64"));
    status=qvariant_cast<QString>(settings.value("status"));
    autoFileGet=qvariant_cast<QString>(settings.value("auto_get_file"));
    ip=QHostAddress(qvariant_cast<QString>(settings.value("ip")));
    port=qvariant_cast<quint16>(settings.value("port"));
    ipFlag=qvariant_cast<bool>(settings.value("show_message_ip"));
    dateTimeFlag=qvariant_cast<bool>(settings.value("show_message_time"));

    if(autoFileGet=="yes") {
        fileAutoAction->setChecked(true);
    }
    if(status=="Доступен") {
        availableAction->setChecked(true);
    } else if(status=="Отошел") {
        afkAction->setChecked(true);
    } else if(status=="Не беспокоить") {
        notDisturbAction->setChecked(true);
    } else {
        if(status.length()>16) {
            QString name=status.left(16)+"...";
            customAction->setText(name);
        } else {
            customAction->setText(status);
        }
        customAction->setChecked(true);
    }

    if(ipFlag) {
        ipAction->setChecked(true);
    }
    if(dateTimeFlag) {
        dateTimeAction->setChecked(true);
    }

}

void Client::on_exitAction_triggered() {
    QSettings settings("/home/user/Desktop/ChatClient.config", QSettings::IniFormat);
    settings.setValue("background_message_color", getBackColor());
    settings.setValue("message_color", getMessageColor());
    settings.setValue("auto_get_file", autoFileGet);
    settings.setValue("username", username);
    settings.setValue("icon_64", icon_64);
    settings.setValue("status", status);
    settings.setValue("ip", ip.toString());
    settings.setValue("port", port);
    settings.setValue("show_message_ip", ipFlag);
    settings.setValue("show_message_time", dateTimeFlag);

    QApplication::quit();
}

void Client::on_photoButton_clicked() {
    QString path=QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/user/Desktop/images", tr("Image Files (*.png)"));
    icon=QImage(path);
    QMessageBox::information(this, tr("Успех!"), tr("Вы успешно поменяли свое фото!"));
    updatedUser();
}

void Client::updatedUser() {
    //формируем 64 от иконки
    QBuffer buffer;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_1);
    buffer.open(QIODevice::WriteOnly);
    icon.save(&buffer, "PNG");
    QString encoded=buffer.data().toBase64();
    icon_64=encoded;
//    qDebug()<<"Попытался отправить иконку: "<<encoded;
    out<<(quint16)0<<(QString)"USER_UPDATED"<<username<<encoded<<status;
    socket->write(block);
}

void Client::on_sendPhoto_clicked() {
    formatSending=false;
    fontButton->hide();
    messageColorButton->hide();
    sendButtonList->setDefaultAction(sendPhoto);

    QString path=QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/user/Desktop/images", tr("Image Files (*.png)"));
    QImage image(path);
    QBuffer buffer;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_1);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QString encoded=buffer.data().toBase64();
    out<<(quint16)0<<(QString)"PHOTO"<<username<<encoded;
//    qDebug()<<"DEBUG \n"<<block;
    socket->write(block);
}

void Client::on_statusAvailable() {
    status="Доступен";
    setWindowTitle(ip.toString()+":"+QString::number(port)+" "+status);
    updatedUser();
}

void Client::on_statusAfk() {
    status="Отошел";
    setWindowTitle(ip.toString()+":"+QString::number(port)+" "+status);
    updatedUser();
}

void Client::on_statusNotDisturb() {
    status="Не беспокоить";
    setWindowTitle(ip.toString()+":"+QString::number(port)+" "+status);
    updatedUser();
}

void Client::on_statusCustomOpen() {
    statusDialog->customStatusLineEdit->setText(status);
    statusDialog->show();
}

void Client::on_statusCustom() {
    status=statusDialog->customStatusLineEdit->text();
    if(status.length()>16) {
        customAction->setText(status.right(16)+"...");
    } else {
        customAction->setText(status);
    }
    QMessageBox::information(mDialog, tr("Успех!"), tr("Вы успешно поменяли свой статус!"));
    setWindowTitle(ip.toString()+":"+QString::number(port)+" "+status);
    updatedUser();
    statusDialog->hide();
}

void Client::on_statusBarButton_clicked() {
    statusBar()->setVisible(!(statusBar()->isVisible()));
}

void Client::setMessageColor(QColor newColor) {
    messageColor=newColor;
}

QColor Client::getMessageColor() {
    return messageColor;
}

void Client::on_actionMessagesColor_triggered() {
    mDialog->show();
}

void Client::on_acceptMessagesColor() {
    messageColor=mDialog->messageColor;
    infoUsersColor=mDialog->infoUsersColor;
    QMessageBox::information(mDialog, tr("Успех!"), tr("Вы успешно поменяли цвет сообщений!"));
    mDialog->hide();
}

void Client::setBackColor(QColor newColor) {
    messagesList->setStyleSheet("background-color: "+newColor.name());
    color=newColor;
}

QColor Client::getBackColor() {
    return color;
}

void Client::on_ipButton_clicked() {
    toggleIpFlag();
}

void Client::on_dateTimeButton_clicked() {
    toggleDateTimeFlag();
}

void Client::toggleIpFlag() {
    ipFlag=!(getIpFlag());
}

bool Client::getIpFlag() {
    return ipFlag;
}

void Client::toggleDateTimeFlag() {
    dateTimeFlag=!(getDateTimeFlag());
}

bool Client::getDateTimeFlag() {
    return dateTimeFlag;
}

void Client::on_infoButton_clicked() {
    infoDialog->show();
}

void Client::on_usernameButton_clicked() {
    uDialog->usernameLineEdit->setText(username);
    uDialog->show();
}

void Client::on_acceptUsername() {
    QString oldUsername=username;
    if (uDialog->usernameLineEdit->text().isEmpty()) {
        QMessageBox::information(uDialog, tr("Пустые поля"), tr("Введите запрос."));
        return;
    }
    username=uDialog->usernameLineEdit->text();

    QMessageBox::information(uDialog, tr("Успех!"), tr("Вы успешно поменяли настройки пользователя!"));
    uDialog->hide();
    updatedUser();
}

void Client::on_serverButton_clicked() {
    sDialog->ipLineEdit->setText(ip.toString());
    sDialog->portLineEdit->setText(QString::number(port));
    sDialog->show();
}

void Client::on_accept() {
    if (sDialog->ipLineEdit->text().isEmpty()||sDialog->portLineEdit->text().isEmpty()) {
        QMessageBox::information(sDialog, tr("Пустые поля"), tr("Введите запрос."));
        return;
    }
    ip=QHostAddress(sDialog->ipLineEdit->text());
    port=sDialog->portLineEdit->text().toUInt();
    QMessageBox::information(sDialog, tr("Успех!"), tr("Вы успешно поменяли настройки сервера!"));
    sDialog->hide();
    serverAction->setText("Сервер: "+ip.toString()+":"+QString::number(port));
}

void Client::on_connectButton_clicked() {
    messagesList->append(tr("<em>Подключение...</em>"));
    connectAction->setEnabled(false);

    socket->abort();
    //Connection:
    socket->connectToHost(ip, port);

    QByteArray packet;

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    icon.save(&buffer, "PNG");
    QString encoded=buffer.data().toBase64();
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_1);
    out<<(quint16)0<<(QString)"USERLIST_UPDATE"<<username<<encoded<<status;
    socket->write(packet);
}

void Client::on_colorChangeButton_clicked() {
    QColor newColor=QColorDialog::getColor(formattedColor);
    if(newColor.isValid()) {
        formattedColor=newColor;
    }
}

void Client::on_fontChangeButton_clicked() {
    bool userChoseFont;
    formattedFont=QFontDialog::getFont(&userChoseFont, message->font(), this);
    if (!userChoseFont)
        return;
}

void Client::on_formatSendButton_clicked() {
    if(formatSending) {
        if(message->text().isEmpty()) {
            QMessageBox::information(this, tr("Пустое сообщение!"), tr("Введите сообщение перед отправкой!"));
            return;
        }
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);
        out<<(quint16)0<<(QString)"FORMATTED_MESSAGE"<<username<<(QString)formattedColor.name()<<formattedFont.family()<<formattedFont.weight()<<message->text();
        socket->write(packet);
        message->clear();
        message->setFocus();
    } else {
        sendButtonList->setDefaultAction(sendFormat);
        fontButton->show();
        messageColorButton->show();
        formatSending=true;
    }
}

void Client::on_sendButton_clicked() {
    formatSending=false;
    fontButton->hide();
    messageColorButton->hide();
    sendButtonList->setDefaultAction(sendButton);

    if(message->text().isEmpty()) {
        QMessageBox::information(this, tr("Пустое сообщение!"), tr("Введите сообщение перед отправкой!"));
        return;
    }

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    QString messageToSend = "<strong>"+ username+ "</strong>" + message->text();
    out << (quint16) 0;
    out << messageToSend;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    socket->write(packet);
    message->clear();
    message->setFocus();
}

void Client::on_message_returnPressed() {
    on_sendButton_clicked();
}

void Client::remove(QLayout *layout) {
    QLayoutItem *child;
    while(layout->count()!=0) {
        child=layout->takeAt(0);
        if(child->layout()!=0) {
            remove(child->layout());
        } else if(child->widget()!=0) {
            delete child->widget();
        }
        delete child;
    }
}

void Client::updateUserList() {
    usersList->clear();

    for(int i=0; i<userList.count(); ++i) {
        QListWidgetItem *item=new QListWidgetItem(QIcon(QPixmap::fromImage(userList[i]->icon)), userList[i]->username);
        usersList->addItem(item);
    }
}

void Client::dataReceived() {
    int serverMes=0;
    QDataStream in(socket);

    if(messageLength ==0) {
        if(socket->bytesAvailable() < (int) sizeof(quint16)) {
//            qDebug()<<"HOW1?";
            return;
        }
//        qDebug()<<"HOW2?";
        in >> messageLength;
//        qDebug()<<messageLength;
    }

    if(messageLength==0) {
//        qDebug()<<"GET FILE MESSAGE LENGTH "<<messageLength;
        QString text;
        in>>text;
//        qDebug()<<"TEXT^ "<<text;
        if(text=="USERSONLINE") {
//            qDebug()<<"USERSONLINE CHEEECK";
            userList.clear();
            QString names;
            QStringList namesS;
            QByteArray images_64;
            QByteArrayList imagesS_64;
            QByteArray image;
                in>>names;
//                qDebug()<<names;
                namesS=names.split(",");
                namesS.removeLast();
                namesS.removeOne(username);
                in>>images_64;
                imagesS_64=images_64.split(',');
                imagesS_64.removeLast();
//                qDebug()<<"images"<<imagesS_64;
                for(std::size_t i=0; i<namesS.count(); ++i) {
                    User *newUser=new User();
                    newUser->username=namesS[i];
                    newUser->icon=QImage::fromData(QByteArray::fromBase64(imagesS_64[i]), "PNG");
                    iconsList.insert(imagesS_64[i],namesS[i]);
                    userList<<newUser;
                }
            updateUserList();
            messageLength = 0;
            return;
            serverMes=1;
        } else if(text=="SIMPLE_MESSAGE") {
            QString message;
            in>>message;
//            qDebug()<<message;
            if(message=="<em>Пользователь отключился.</em>") {
                QString messageNew="Пользователь отключился.";
                QDomElement newLog=log(doc, QDateTime::currentDateTime().toString(), socket->peerAddress().toString(), "SERVER", "serverMessage", messageNew);
                domElement.appendChild(newLog);
                messagesList->append(message);
                updatedUser();
                messageLength=0;
                return;
            } else if(message=="<em>Пользователь подключился!</em>") {
                QString messageNew="Пользователь подключился!";
                QDomElement newLog=log(doc, QDateTime::currentDateTime().toString(), socket->peerAddress().toString(), "SERVER", "serverMessage", messageNew);
                domElement.appendChild(newLog);
                messagesList->append(message);
                messageLength=0;
                return;
            }
            int needPos=message.indexOf(">");
            int needPos2=message.indexOf("<", needPos);
            int needPos3=message.indexOf("[");
            int needPos4=message.indexOf("]");
            int needPos5=message.indexOf(">", needPos2);
            QString name=message.mid(needPos+1, needPos2-needPos-1);
            QString ipString=message.mid(needPos3+1,needPos4-1);

            QDomElement newLog=log(doc, QDateTime::currentDateTime().toString(), ipString, name, "simpleMessage", message.mid(needPos5+1, message.count()));
            domElement.appendChild(newLog);

            name.prepend("<strong><font color="+infoUsersColor.name()+">");
            name.append("</font></strong>");
            ipString.prepend("<font color="+infoUsersColor.name()+">[");
            ipString.append("]</font>");
            QString messageString=message.mid(needPos5+1, message.count());
            messageString.prepend("<font color="+messageColor.name()+">");
            messageString.append("</font>");
            QString dateTimeString="";
            QString finalMessage="";
            finalMessage.append(name+": ");
            finalMessage.append(messageString);
            if(ipFlag) {
                finalMessage.prepend(ipString);
            }
            if(!ipFlag) {
                int firstIp=message.indexOf("[");
                int secondIp=message.indexOf("]");
                message.remove(firstIp, secondIp+1);
                message.prepend(" ");
            }
            if(dateTimeFlag) {
                dateTimeString="\n"+QString::number(QDateTime::currentDateTime().date().day())+"."+QString::number(QDateTime::currentDateTime().date().month())+"."+QString::number(QDateTime::currentDateTime().date().year())+" "+QString::number(QDateTime::currentDateTime().time().hour())+":"+QString::number(QDateTime::currentDateTime().time().minute())+"  ";
                dateTimeString.prepend("<font color="+infoUsersColor.name()+">");
                dateTimeString.append("</font>");
                finalMessage.prepend(dateTimeString);
            }
            message.prepend("<font color="+messageColor.name()+">");
            message.append("<\font>");
            if(status!="Не беспокоить"&&name!=username) {
//                QMediaPlayer *player=new QMediaPlayer;
//                player->setMedia(QUrl::fromLocalFile("/home/user/Downloads/signal.mp3"));
//                player->setVolume(50);
//                player->play();
                QSound *s=new QSound("./messageSignal.wav");
                s->play();
            }
            messagesList->append(finalMessage);
            messageLength = 0;
            serverMes=1;
            return;

        } else if(text=="DISCONNECTEDUSER") {
            QString usernameDisc;
            in>>usernameDisc;
            for(std::size_t i=0; i<userList.count(); ++i) {
                if(userList[i]->username==usernameDisc) {
                    userList.removeAt(i);
                }
            }
            for(std::size_t i=0; i<usersList->count(); ++i) {
                if(usersList->item(i)->text()==usernameDisc) {
                    usersList->removeItemWidget(usersList->item(i));
                }
            }
        } else if(text=="USERLIST_UPDATED") {
//            qDebug()<<"USERLIST_UPDATED CHEEEECK";
            userList.clear();
            QString all_names;
            QByteArray all_bytes;
            QStringList ips;
            QStringList times;
            QStringList statuses;
            in>>all_names;
            QByteArrayList bytes;
            in>>bytes;
            in>>ips;
            in>>times;
            in>>statuses;
//            qDebug()<<"ips"<<ips;
//            qDebug()<<"times"<<times;
//            qDebug()<<"statuses"<<statuses;
            QStringList names=all_names.split(",");
            names.removeLast();
            User *deletedLaterUser;
            for(int i=0; i<names.count(); ++i) {
                User *newUser=new User();
                newUser->username=names[i];
                newUser->icon=QImage::fromData(QByteArray::fromBase64(bytes[i]), "PNG");
                newUser->ip=ips[i];
                newUser->connected_time=times[i];
                newUser->status=statuses[i];
                qDebug()<<"Добавлен пользователь"<<newUser->username;
                qDebug()<<ips[i]<<times[i]<<statuses[i];
                if(username==names[i])
                    deletedLaterUser=newUser;
                userList<<newUser;
            }
            userList.removeOne(deletedLaterUser);
            updateUserList();
            messageLength = 0;

            //TODO баг может из-за этого? Но тогда другой вылетает, не правда ли, creature of meat and bone?
//            return;
        }
        //можно удалить по идее, DELETE
        else if(text=="FILESENDEDC") {
//            qDebug()<<"FILESENDEC CHEEEECK";
            if(fDialog->isVisible())
                fDialog->progressBar->setValue(100);
            QMessageBox::information(this, tr("Успех!"), tr("Вы успешно отправили файл!"));
            ++amountSendedFiles;
            countPercentSend();
            messageLength = 0;
            return;
        } else if(text=="ASKFORFILEGET") {
            QString filename;
            QString username;
            in>>filename;
            in>>username;
            if(status!="Не беспокоить") {
                QSound *s=new QSound("./messageSignal.wav");
                s->play();
//                QMediaPlayer *player=new QMediaPlayer;
//                player->setMedia(QUrl::fromLocalFile("/home/user/Downloads/signal.mp3"));
//                player->setVolume(50);
//                player->play();
            }

            if(autoFileGet=="yes") {
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_1);
                out<<(quint16)0<<(QString)"FILEGETACCEPT";
                socket->write(block);


            } else if(autoFileGet=="no") {
                QMessageBox::StandardButton msgBox;
                msgBox=QMessageBox::question(this, "Пришел файл", "Вы действительно хотите принять файл с названием "+filename+" от "+username+"?", QMessageBox::Yes|QMessageBox::No);
                if(msgBox==QMessageBox::Yes) {
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_1);
                    out<<(quint16)0<<(QString)"FILEGETACCEPT";
                    socket->write(block);

                    FileSendDialog *fDialogNew=new FileSendDialog(this, openedAmount);
                    fileSendList.append(fDialogNew);
                    fDialogNew->selectFileButton->hide();
                    fDialogNew->sendButton->hide();
                    fDialogNew->fileLabel->setText(filename);
                    fDialogNew->show();
                    fDialog=fDialogNew;
                    fDialog->progressBar->setValue(100);
                } else {
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_1);
                    out<<(quint16)0<<(QString)"FILEGETDECLINE";
                    socket->write(block);
                }
            }
            messageLength = 0;
            return;
        } else if(text=="FILEFORCLIENT") {
//            qDebug()<<"FILEFORCLIENT";

            QString usernameSend;
            QString filename;
            quint16 size;
            QByteArray fileBlock;
            in>>usernameSend;
//            qDebug()<<usernameSend;
            in>>filename;
//            qDebug()<<filename;
            in>>size;
//            qDebug()<<size;
            in>>fileBlock;
//            qDebug()<<fileBlock;

            ++amountGetFiles;
            amountGettingFiles->setText("Кол-во получаемых файлов: "+QString::number(amountGetFiles));
            countPercentGet();
            fileBlock.remove(0,4);

            sizeBytesGet+=size;
            fileGettingSize->setText(sizeConvert(sizeBytesGet));
            QString md5=QString(QCryptographicHash::hash((fileBlock), QCryptographicHash::Md5).toHex());

            if(QFileInfo("/home/user/DownloadsFromChat/"+filename+".txt").exists()) {
                int i=1;
                bool numberFound=false;
                while(numberFound==false) {
                    QString filename_copy=filename;
                    filename_copy.append(QString::number(i)+".txt");
                    if(!QFileInfo("/home/user/DownloadsFromChat/"+filename_copy).exists()) {
                        numberFound=true;
                        filename=filename_copy;
                        break;
                    }
                    ++i;
                }
                QFile file("/home/user/DownloadsFromChat/"+filename);
                if(file.open(QIODevice::ReadWrite)) {
                    QTextStream stream(&file);
                    stream<<fileBlock;
                    file.close();

                    ++amountGettedFiles;
                    countPercentGet();
                }
            } else {
                filename.append(".txt");
                QFile file("/home/user/DownloadsFromChat/"+filename);
                if(file.open(QIODevice::ReadWrite)) {
                    QTextStream stream(&file);
                    stream<<fileBlock;
                    file.close();

                    ++amountGettedFiles;
                    countPercentGet();
                }
            }

            QString ipString;
            for(std::size_t i=0; i<userList.count(); ++i) {
                if(userList[i]->username==usernameSend) {
                    ipString=userList[i]->ip;
                }
            }
            QDomElement newLog=log(doc, QDateTime::currentDateTime().toString(), ipString, usernameSend, "fileMessage", filename+", "+md5);
            domElement.appendChild(newLog);
            messagesList->append("Вам был отправлен файл от "+usernameSend+". Имя файла: "+filename+". Размер файла: "+QString::number(size)+"б.");
            messageLength = 0;
            return;
        } else if(text=="SERVERGOTFILE") {
            //
            messageLength = 0;
            return;

        } else if(text=="CLIENTASKEDFORFILE") {
            //
            messageLength = 0;
            return;
        } else if(text=="CLIENTACCEPTFILE") {
//            qDebug()<<"CLIENTACCEPTFILE";
            fDialog->progressBar->setValue(100);
            ++amountSendedFiles;
            countPercentSend();

            messageLength = 0;
            return;
        } else if(text=="CLIENTDECLINEFILE") {
//            qDebug()<<"CLIENTDECLINEFILE";
            //можно дописать warning
            messageLength = 0;
            return;
        }
        //Тоже можно удалить, DELETE
        else if(text=="FILEFORYOU") {
//            qDebug()<<"FILEFORYOU CHEEEECK";
            ++amountGetFiles;
            amountGettingFiles->setText("Кол-во получаемых файлов: "+QString::number(amountGetFiles));
            countPercentGet();
            QString filename;
            QByteArray block;
            QString username;
            in>>filename;
            in>>block;
            in>>username;
            block.remove(0,4);
            int bsize=block.count();
            sizeBytesGet+=bsize;
            fileGettingSize->setText(sizeConvert(sizeBytesGet));
//            qDebug()<<"RECEIVED FILE!!"<<filename;
//            qDebug()<<block;
            QString md5=QString(QCryptographicHash::hash((block), QCryptographicHash::Md5).toHex());

            if(status!="Не беспокоить") {
                QMediaPlayer *player=new QMediaPlayer;
                player->setMedia(QUrl::fromLocalFile("/home/user/Downloads/signal.mp3"));
                player->setVolume(50);
                player->play();
            }

            FileSendDialog *gDialog=new FileSendDialog(this);
            if(autoFileGet=="no") {
                qDebug()<<"OPEN INFO WINDOW";
                gDialog->show();
                gDialog->fileLabel->setText("Вы согласны принять файл: "+filename+".txt."+" От "+username+". Размером в "+QString::number(bsize));
                gDialog->acceptButton->show();
                gDialog->noButton->show();
                gDialog->selectFileButton->hide();
                gDialog->sendButton->hide();
                gDialog->progressBar->hide();
                gDialog->packet=block;
                gDialog->filename_new=filename;

                if(gDialog->fileSavingReady) {
                    gDialog->progressBar->setValue(60);
                    QByteArray blockForUser;
                    QDataStream out(&blockForUser, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_1);
                    out<<(quint16)0<<(QString)"FILESENDEDC";
                    socket->write(blockForUser);

                    gDialog->progressBar->setValue(80);
                    ++amountGettedFiles;
                    countPercentGet();

                    gDialog->progressBar->setValue(100);
                    messagesList->append("Вам был отправлен файл от "+username+". Имя файла: "+filename+". Размер файла: "+bsize+"б.");
                }
                messageLength = 0;
                return;
            }

            if(QFileInfo("/home/user/DownloadsFromChat/"+filename+".txt").exists()) {
                int i=1;
                bool numberFound=false;
                while(numberFound==false) {
                    QString filename_copy=filename;
                    filename_copy.append(QString::number(i)+".txt");
                    if(!QFileInfo("/home/user/DownloadsFromChat/"+filename_copy).exists()) {
                        numberFound=true;
                        filename=filename_copy;
                        break;
                    }
                    ++i;
                }
                QFile file("/home/user/DownloadsFromChat/"+filename);
                if(file.open(QIODevice::ReadWrite)) {
                    QTextStream stream(&file);
                    stream<<block;
                    file.close();

                    if(gDialog->clicked==1)
                        gDialog->progressBar->setValue(60);

                    QByteArray blockForUser;
                    QDataStream out(&blockForUser, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_1);
                    out<<(quint16)0<<(QString)"FILESENDEDC";
                    if(gDialog->clicked==1)
                        gDialog->progressBar->setValue(90);
                    ++amountGettedFiles;
                    countPercentGet();
                    if(autoFileGet=="yes") {
                        messagesList->append("Вам был отправлен файл от "+username+". Имя файла: "+filename+". Размер файла: "+file.size()+"б.");
                        messageLength = 0;
                        return;
                    }
                }
            } else {
                filename.append(".txt");
                QFile file("/home/user/DownloadsFromChat/"+filename);
                if(file.open(QIODevice::ReadWrite)) {
                    QTextStream stream(&file);
                    stream<<block;
                    file.close();

                    QByteArray blockForUser;
                    QDataStream out(&blockForUser, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_1);
                    out<<(quint16)0<<(QString)"FILESENDEDC";
                    ++amountGettedFiles;
                    countPercentGet();
                    if(autoFileGet=="yes") {
                        messagesList->append("Вам был отправлен файл от "+username+". Имя файла: "+filename+". Размер файла: "+file.size()+"б.");
                        messageLength = 0;
                        return;
                    }
                }
            }

            QString ipString;
            for(std::size_t i=0; i<userList.count(); ++i) {
                if(userList[i]->username==username) {
                    ipString=userList[i]->ip;
                }
            }
            QDomElement newLog=log(doc, QDateTime::currentDateTime().toString(), ipString, username, "fileMessage", filename+", "+md5);
            domElement.appendChild(newLog);

        }  else if(text=="FORMATTED_MESSAGE") {
//            qDebug()<<"FORMATTED_MESSAGE CHEEEECK";
            if(status!="Не беспокоить") {
                /*QMediaPlayer *player=new QMediaPlayer;
                player->setMedia(QUrl::fromLocalFile("/home/user/Downloads/signal.mp3"));
                player->setVolume(50);
                player->play();*/
                QSound *s=new QSound("./messageSignal.wav");
                s->play();
            }
            QString username;
            QString colorStr;
            QString fontName;
            int fontWeight;
            QString ip;
            QString message;
            in>>username;
            in>>colorStr;
            in>>fontName;
            in>>fontWeight;
            in>>ip;
            in>>message;

            QString infoFormat="("+colorStr+","+fontName+","+fontWeight+") ";
            QDomElement newLog=log(doc, QDateTime::currentDateTime().toString(), ip, username, "formattedMessage", infoFormat+message);
            domElement.appendChild(newLog);

            QString dateTimeString;
            username.prepend("<strong><font color="+infoUsersColor.name()+">");
            username.append("</font></strong>: ");
            dateTimeString="\n"+QString::number(QDateTime::currentDateTime().date().day())+"."+QString::number(QDateTime::currentDateTime().date().month())+"."+QString::number(QDateTime::currentDateTime().date().year())+" "+QString::number(QDateTime::currentDateTime().time().hour())+":"+QString::number(QDateTime::currentDateTime().time().minute())+"  ";
            if(messageColor!=Qt::black) {
                message.prepend("<font color="+messageColor.name()+">");
                message.append("</font>");
            } else {
                message.prepend("<font color="+colorStr+" font-size="+fontWeight+" font-family="+fontName+">");
                message.append("</font>");
            }
            message.prepend(username);
            if(ipFlag) {
                ip.prepend("<font color="+infoUsersColor.name()+">[");
                ip.append("]</font> ");
                message.prepend(ip);
            }
            if(dateTimeFlag) {
                dateTimeString.prepend("<font color="+infoUsersColor.name()+">");
                dateTimeString.append("</font>");
                message.prepend(dateTimeString);
            }
            messagesList->append(message);
            messageLength = 0;
            return;

        }
        //Тоже, вроде, можно убрать, CHECK
        else if(text=="UPDATED_USER_ICON") {
//            qDebug()<<"ICON UPDATED";
            QString name;
            QByteArray icon_64;
            in>>name;
            in>>icon_64;
//            qDebug()<<icon_64;
//            qDebug()<<"NAME"<<name;
            for(std::size_t i=0; i<userList.count(); ++i) {
                if(userList[i]->username==name) {
                    userList[i]->icon=QImage::fromData(QByteArray::fromBase64(icon_64),"PNG");
//                    qDebug()<<icon_64;
//                    qDebug()<<"УСТАНОВИЛИ КАРТИНКУ ПОЛЬЗОВАТЕЛЮ В СПИСКЕ";
                }
            }
            updateUserList();
            messageLength = 0;
            serverMes=1;
            return;
        } else if(text=="PHOTO") {
//            qDebug()<<"PHOTO CHEEEECK";
            QString author;
            QByteArray image_64;
            QString ipString;
            in>>author;
            in>>ipString;
            in>>image_64;
            qDebug()<<"SENDED PHOTO"<<image_64;
            QImage image=QImage::fromData(QByteArray::fromBase64(image_64), "PNG");
            QImage image_new=image.scaled(320,240);
            QTextCursor newCursor=messagesList->textCursor();
            newCursor.movePosition(QTextCursor::End);

            photosList.insert(newCursor.position(), image);

            if(status!="Не беспокоить") {
                QMediaPlayer *player=new QMediaPlayer;
//                player->setMedia(QUrl::fromLocalFile("/home/user/Downloads/signal.mp3"));
//                player->setVolume(50);
//                player->play();
                QSound *s=new QSound("./messageSignal.wav");
                s->play();
            }
            messagesList->setTextCursor(newCursor);
            messagesList->textCursor().insertBlock();
            messagesList->textCursor().insertImage(image_new);

            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);
            image_new.save(&buffer, "PNG");
            QString encoded=buffer.data().toBase64();
            QDomElement newLog=log(doc, QDateTime::currentDateTime().toString(), ipString, author, "imageMessage", encoded);
            domElement.appendChild(newLog);

            messageLength = 0;
            return;
        }
    }
}

void Client::connectUser() {
    messagesList->append(tr("<em>Успешное подключение!</em>"));
    connectAction->setEnabled(true);

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    QString content="";
    QString messageToSend = "[USERNAME] "+username +",[STATUS]"+status+",[AFG]"+autoFileGet+",[ICON]"+content;
    out << (quint16) 0;
    out << messageToSend;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));
    socket->write(packet);

    setWindowTitle(ip.toString()+":"+QString::number(port)+" "+status);

}

void Client::preLogOut() {
    messagesList->append(tr("<em>Отключен от сервера.</em>"));
    setWindowTitle("Клиент");
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_1);
    out<<(quint16)0<<(QString)"DISCONNECT"<<username;
    socket->write(block);
    userList.clear();
    usersList->clear();
    logOut();
}

void Client::logOut() {
    socket->disconnectFromHost();
}

void Client::socketError(QAbstractSocket::SocketError error) {
    switch(error) {
        case QAbstractSocket::HostNotFoundError:
            messagesList->append(tr("<em>Error: сервер не найден.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            messagesList->append(tr("<em>Error: подключение прервано.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            messagesList->append(tr("<em>Error: подключение закрыто.</em>"));
            break;
        default:
            messagesList->append(tr("<em>Error: ") + socket->errorString() + tr("</em>"));
    }

    connectAction->setEnabled(true);
}

void Client::on_actionBackColor_triggered() {
    QColor newColor=QColorDialog::getColor(getBackColor());
    if(newColor.isValid()) {
        setBackColor(newColor);
    }
}

void Client::on_showContextMenuUsers(const QPoint &point) {
    QPoint globalPos=usersList->mapToGlobal(point);
    QMenu contextMenu;
    contextMenu.addAction("Информация", this, SLOT(openInfoUser()));
    contextMenu.addAction("Отправить файл", this, SLOT(openFileSending()));
    contextMenu.exec(globalPos);
}

void Client::on_showContextMenuMessagesListUsers(const QPoint &point) {
    QPoint globalPos=messagesList->mapToGlobal(point);
    QMenu contextMenu;
    contextMenu.addAction("Открыть в полном размере", this, SLOT(openFullSizeImage()));
    contextMenu.addAction("Сохранить изображение", this, SLOT(saveImage()));
    contextMenu.exec(globalPos);
    lastPos=messagesList->textCursor().position();
//    qDebug()<<"POSTITINO"<<lastPos;
}

void Client::openFullSizeImage() {
    QImage image;
    QBuffer buffer;
    ImageFullDialog *iDialog=new ImageFullDialog(this);
    buffer.open(QIODevice::WriteOnly);
    if(photosList.value(lastPos).isNull()) {
        if(photosList.value(lastPos-1).isNull()) {
            if(photosList.value(lastPos-2).isNull()) {
                if(photosList.value(lastPos-3).isNull()) {
                    if(photosList.value(lastPos-4).isNull()) {
                        if(photosList.value(lastPos-5).isNull()) {
                            //nothing
                        } else {
                            photosList.value(lastPos-5).save(&buffer, "PNG");
                            image=photosList.value(lastPos-5);
                        }
                    } else {
                        photosList.value(lastPos-4).save(&buffer, "PNG");
                        image=photosList.value(lastPos-4);
                    }
                } else {
                    photosList.value(lastPos-3).save(&buffer, "PNG");
                    image=photosList.value(lastPos-3);
                }
            } else {
                photosList.value(lastPos-2).save(&buffer, "PNG");
                image=photosList.value(lastPos-2);
            }
        } else {
            photosList.value(lastPos-1).save(&buffer, "PNG");
            image=photosList.value(lastPos-1);
        }
    } else {
        photosList.value(lastPos).save(&buffer, "PNG");
        image=photosList.value(lastPos);
    }
    QLabel *image_label=new QLabel(this);
    image_label->setPixmap(QPixmap::fromImage(image));

    iDialog->imageFullLabel->setPixmap(QPixmap::fromImage(image));
    iDialog->imageFullLabel->setWindowFlags(Qt::Window);
    iDialog->verticalLayout->addWidget(image_label);
    iDialog->show();
    QString encoded=buffer.data().toBase64();
//    qDebug()<<encoded<<"YES!";
}

void Client::saveImage() {
    QImage image;
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    if(photosList.value(lastPos).isNull()) {
        if(photosList.value(lastPos-1).isNull()) {
            if(photosList.value(lastPos-2).isNull()) {
                if(photosList.value(lastPos-3).isNull()) {
                    if(photosList.value(lastPos-4).isNull()) {
                        if(photosList.value(lastPos-5).isNull()) {
                            //nothing
                        } else {
                            photosList.value(lastPos-5).save(&buffer, "PNG");
                            image=photosList.value(lastPos-5);
                        }
                    } else {
                        photosList.value(lastPos-4).save(&buffer, "PNG");
                        image=photosList.value(lastPos-4);
                    }
                } else {
                    photosList.value(lastPos-3).save(&buffer, "PNG");
                    image=photosList.value(lastPos-3);
                }
            } else {
                photosList.value(lastPos-2).save(&buffer, "PNG");
                image=photosList.value(lastPos-2);
            }
        } else {
            photosList.value(lastPos-1).save(&buffer, "PNG");
            image=photosList.value(lastPos-1);
        }
    } else {
        photosList.value(lastPos).save(&buffer, "PNG");
        image=photosList.value(lastPos);
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"), QString(), tr("Images (*.png)"));
    if(!fileName.isEmpty()) {
        image.save(fileName);
        QMessageBox::information(this, tr("Успех!"), tr("Успешное сохранение изображения!"));
    }
}

void Client::openInfoUser() {
    QString name=usersList->currentItem()->text();
    QString dateTime;
    QString ip;
    QString status;
    for(std::size_t i=0; i<userList.count(); ++i) {
        if(userList[i]->username==name) {
            dateTime=userList[i]->connected_time;
            ip=userList[i]->ip;
            status=userList[i]->status;
        }
    }
    UserInfoDialog *userInfoDialog=new UserInfoDialog(this, name, ip, dateTime, status);
    userInfoDialog->show();
}

void Client::openFileSending() {
    FileSendDialog *fDialogNew=new FileSendDialog(this, openedAmount);
    fileSendList.append(fDialogNew);
    fDialogNew->show();
    fDialog=fDialogNew;
    ++openedAmount;
    ++amountSendFiles;
    amountSendingFiles->setText("Кол-во отправляемых файлов: "+QString::number(amountSendFiles));
    countPercentSend();
}

QString Client::sizeConvert(int bsize) {
    int bytes=bsize;
    float kbytes=bytes/1024;
    float mbytes=bytes/1024;
    QString result="Общий размер файлов: "+QString::number(bytes)+"б/"+QString::number(kbytes)+"Кб/"+QString::number(mbytes)+"Мб";
    return result;
}

void Client::countPercentSend() {
    int percent=(100*amountSendedFiles)/amountSendFiles;
    percentProgressSend=percent;
    QString label="Прогресс отправки файлов: "+QString::number(percent)+"%";
    fileSendingProgress->setText(label);
}

void Client::countPercentGet() {
    int percent=(100*amountGettedFiles)/amountGetFiles;
    percentProgressGet=percent;
    QString label="Прогресс получения файлов: "+QString::number(percent)+"%";
    fileGettingProgress->setText(label);
}

void Client::on_sendFile() {
    sizeBytesSend+=fDialog->bytes;
    fileSendingSize->setText(sizeConvert(sizeBytesSend));
    fDialog->progressBar->setValue(35);
    QByteArray block=fDialog->file_bity;
    QString filename=fDialog->filename;
    quint16 size=fDialog->bytes;
    QString username=usersList->currentItem()->text();
    fDialog->progressBar->setValue(50);
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_1);
    out<<(quint16)0<<(QString)"FILEFORSERVER"<<username<<filename<<size<<block;
//    qDebug()<<filename<<block;
    fDialog->progressBar->setValue(75);
    socket->write(packet);
    fDialog->progressBar->setValue(90);
}

QDomElement Client::makeElement(QDomDocument *domDoc, const QString &strName, const QString &strAttr=QString::null, const QString &Text=QString::null) {
    QDomElement domElement=domDoc->createElement(strName);
    if(!strAttr.isEmpty()) {
        QDomAttr domAttr=domDoc->createAttribute("number");
        domAttr.setValue(strAttr);
        domElement.setAttributeNode(domAttr);
    }
    if(!Text.isEmpty()) {
        QDomText domText=domDoc->createTextNode(Text);
        domElement.appendChild(domText);
    }
    return domElement;
}


QDomElement Client::log(QDomDocument *domDoc, const QString &dateTime, const QString &ip, const QString &name, const QString &type, const QString &message) {
    static int n=1;
    QDomElement domElement=makeElement(domDoc, "log", QString().setNum(n));
    domElement.appendChild(makeElement(domDoc, "name", "", name));
    domElement.appendChild(makeElement(domDoc, "ip", "", ip));
    domElement.appendChild(makeElement(domDoc, "dateAndTime", "",dateTime));
    if(type=="simpleMessage") {
        domElement.appendChild(makeElement(domDoc, "message", "", message));
    } else if(type=="formattedMessage") {
        domElement.appendChild(makeElement(domDoc, "formattedMessage", "", message));
    } else if(type=="imageMessage") {
        domElement.appendChild(makeElement(domDoc, "imageMessage", "", message));
    } else if(type=="fileMessage") {
        domElement.appendChild(makeElement(domDoc, "fileMessage", "", message));
    } else if(type=="serverMessage") {
        domElement.appendChild(makeElement(domDoc, "serverMessage", "", message));
    }
    ++n;
    return domElement;
}

void Client::saveXMLLogs() {
    QFile file("logs.xml");
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream(&file)<<doc->toString();
        file.close();
    }
}

void Client::on_saveLogs() {
    saveXMLLogs();
}

void Client::on_autoFileGetButton_clicked() {
    if(fileAutoAction->isChecked()) {
        autoFileGet="yes";
    } else {
        autoFileGet="no";
    }
}
