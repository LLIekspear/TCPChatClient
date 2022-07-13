#include "customStatusDialog.h"
#include <QHBoxLayout>

CustomStatusDialog::CustomStatusDialog(QWidget *parent) : QDialog(parent) {
    initializeWidgets();

    // чтобы редактор оставался активен, пока открыто окно
    setFocusProxy(customStatusLineEdit);

    // устанавливаем все виджеты и слои
    initializeLayout();
    setWindowTitle(tr("Настройки статуса"));
    connect(acceptButton, SIGNAL(clicked()), parent, SLOT(on_statusCustom()));
}

CustomStatusDialog::~CustomStatusDialog() {
    delete customStatusLabel;
    delete customStatusLineEdit;
    delete acceptButton;
    delete statusHorizontalLayout;
    delete verticalLayout;
}

void CustomStatusDialog::initializeWidgets() {
    customStatusLabel=new QLabel(tr("Статус: "));
    customStatusLineEdit=new QLineEdit();
    acceptButton=new QPushButton(tr("&Применить"));
}

void CustomStatusDialog::initializeLayout() {
    statusHorizontalLayout=new QHBoxLayout();
    verticalLayout=new QVBoxLayout();
    verticalLayout->addLayout(statusHorizontalLayout);
    verticalLayout->addWidget(acceptButton);
    statusHorizontalLayout->addWidget(customStatusLabel);
    statusHorizontalLayout->addWidget(customStatusLineEdit);
    setLayout(verticalLayout);
}

void CustomStatusDialog::on_acceptButton_clicked() {
    QString username=customStatusLineEdit->text();

    if (username.isEmpty()) {
        QMessageBox::information(this, tr("Пустые поля"), tr("Введите запрос."));
        return;
    }
}
