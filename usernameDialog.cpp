#include "usernameDialog.h"
#include <QHBoxLayout>

UsernameDialog::UsernameDialog(QWidget *parent) : QDialog(parent) {
    initializeWidgets();

    // чтобы редактор оставался активен, пока открыто окно
    setFocusProxy(usernameLineEdit);

    // устанавливаем все виджеты и слои
    initializeLayout();
    setWindowTitle(tr("Настройки пользователя"));
    connect(acceptButton, SIGNAL(clicked()), parent, SLOT(on_acceptUsername()));
}

UsernameDialog::~UsernameDialog() {
    delete usernameLabel;
    delete usernameLineEdit;
    delete acceptButton;
    delete usernameHorizontalLayout;
    delete verticalLayout;
}

void UsernameDialog::initializeWidgets() {
    usernameLabel=new QLabel(tr("Имя пользователя: "));
    usernameLineEdit=new QLineEdit();
    acceptButton=new QPushButton(tr("&Применить"));
}

void UsernameDialog::initializeLayout() {
    usernameHorizontalLayout=new QHBoxLayout();
    verticalLayout=new QVBoxLayout();
    verticalLayout->addLayout(usernameHorizontalLayout);
    verticalLayout->addWidget(acceptButton);
    usernameHorizontalLayout->addWidget(usernameLabel);
    usernameHorizontalLayout->addWidget(usernameLineEdit);
    setLayout(verticalLayout);
}

void UsernameDialog::on_acceptButton_clicked() {
    QString username=usernameLineEdit->text();

    if (username.isEmpty()) {
        QMessageBox::information(this, tr("Пустые поля"), tr("Введите запрос."));
        return;
    }
}
