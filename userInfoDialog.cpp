#include "userInfoDialog.h"
#include <QHBoxLayout>

UserInfoDialog::UserInfoDialog(QWidget *parent, QString name, QString ip, QString dateTime, QString status) : QDialog(parent) {
    initializeWidgets(ip, dateTime, status);

    setModal(true);

    // устанавливаем все виджеты и слои
    initializeLayout();
    setWindowTitle(name);
    connect(returnButton, SIGNAL(clicked()), this, SLOT(on_returnButton_clicked()));
}

UserInfoDialog::~UserInfoDialog() {
    delete ipLabel;
    delete dateTimeLabel;
    delete statusLabel;
    delete verticalLayout;
}

void UserInfoDialog::initializeWidgets(QString ip, QString dateTime, QString status) {
    ipLabel=new QLabel(tr("IP: ")+ip);
    dateTimeLabel=new QLabel(tr("Время и дата подключения: ")+dateTime);
    statusLabel=new QLabel(tr("Статус: ")+status);
    returnButton=new QPushButton(tr("&Вернуться"));
}

void UserInfoDialog::initializeLayout() {
    verticalLayout=new QVBoxLayout();
    verticalLayout->addWidget(ipLabel);
    verticalLayout->addWidget(dateTimeLabel);
    verticalLayout->addWidget(statusLabel);
    verticalLayout->addWidget(returnButton);
    setLayout(verticalLayout);
}

void UserInfoDialog::on_returnButton_clicked() {
    hide();
    close();
}
