#include "messagesColor.h"
#include <QHBoxLayout>

MessagesDialog::MessagesDialog(QWidget *parent) : QDialog(parent) {
    initializeWidgets();
    setModal(true);

    // устанавливаем все виджеты и слои
    initializeLayout();
    setWindowTitle(tr("Цвет сообщений"));
    connect(acceptButton, SIGNAL(clicked()), parent, SLOT(on_acceptMessagesColor()));
    connect(messageColorButton, SIGNAL(clicked()), this, SLOT(on_messageColorButton_clicked()));
    connect(infoUserColorButton, SIGNAL(clicked()), this, SLOT(on_infoUserColorButton_clicked()));
}

MessagesDialog::~MessagesDialog() {
    delete acceptButton;
    delete messageColorButton;
    delete infoUserColorButton;
    delete messageColorHorizontalLayout;
    delete infoUserHorizontalLayout;
    delete verticalLayout;
}

void MessagesDialog::initializeWidgets() {
    acceptButton=new QPushButton(tr("&Применить"));
    messageColorButton=new QPushButton(tr("Выбрать цвет текста сообщений"));
    infoUserColorButton=new QPushButton(tr("Выбрать цвет информации о пользователе в сообщении"));

}

void MessagesDialog::initializeLayout() {
    messageColorHorizontalLayout=new QHBoxLayout();
    infoUserHorizontalLayout=new QHBoxLayout();
    verticalLayout=new QVBoxLayout();
    verticalLayout->addLayout(messageColorHorizontalLayout);
    verticalLayout->addLayout(infoUserHorizontalLayout);
    verticalLayout->addWidget(acceptButton);
    messageColorHorizontalLayout->addWidget(messageColorButton);
    infoUserHorizontalLayout->addWidget(infoUserColorButton);
    setLayout(verticalLayout);
}

void MessagesDialog::on_messageColorButton_clicked() {
    QColor newColor=QColorDialog::getColor(messageColor);
    if(newColor.isValid()) {
        messageColor=newColor;
    }
}

void MessagesDialog::on_infoUserColorButton_clicked() {
    QColor newColor=QColorDialog::getColor(infoUsersColor);
    if(newColor.isValid()) {
        infoUsersColor=newColor;
    }
}

void MessagesDialog::on_acceptButton_clicked() {
//

}
