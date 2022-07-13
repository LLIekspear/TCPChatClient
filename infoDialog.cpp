#include "infoDialog.h"
#include <QHBoxLayout>

InfoDialog::InfoDialog(QWidget *parent)
    : QDialog(parent) {
    setModal(true);
    initializeWidgets();
    initializeLayout();
    setWindowTitle(tr("О программе"));
    resize(1000,700);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(on_closeButton_clicked()));
}

InfoDialog::~InfoDialog() {
    delete infoLabel;
    delete dateBuildLabel;
    delete findHorizontalLayout;
    delete dateBuildHorizontalLayout;
    delete verticalLayout;
}

void InfoDialog::initializeWidgets() {
    infoLabel=new QLabel(tr("Автор: Олег Фролов Витальевич СКБ201"));
    infoLabel->setStyleSheet("background-image:url(/home/user/Desktop/photo.jpg); background-repeat: no-repeat; background-size: 70% 50%; background-position: center;");
    infoLabel->resize(400,400);
    dateBuildLabel=new QLabel(tr("Дата сборки: ")+__DATE__+tr(" в ")+__TIME__);
    qtversionbuildLabel=new QLabel(tr("Версия Qt на момент сборки: 5.15.0"));
    qtversionstartedLabel=new QLabel(tr("Версия Qt на момент запуска: ")+QT_VERSION_STR);
    closeButton=new QPushButton("Закрыть");
}

void InfoDialog::initializeLayout() {
    findHorizontalLayout=new QHBoxLayout();
    dateBuildHorizontalLayout=new QHBoxLayout();
    qtversionbuildHorizontalLayout=new QHBoxLayout();
    qtversionstartedHorizontalLayout=new QHBoxLayout();
    verticalLayout=new QVBoxLayout();

    verticalLayout->addLayout(findHorizontalLayout);
    verticalLayout->addLayout(dateBuildHorizontalLayout);
    verticalLayout->addLayout(qtversionbuildHorizontalLayout);
    verticalLayout->addLayout(qtversionstartedHorizontalLayout);

    findHorizontalLayout->addWidget(infoLabel);
    dateBuildHorizontalLayout->addWidget(dateBuildLabel);
    qtversionbuildHorizontalLayout->addWidget(qtversionbuildLabel);
    qtversionstartedHorizontalLayout->addWidget(qtversionstartedLabel);
    qtversionstartedHorizontalLayout->addWidget(closeButton);

    setLayout(verticalLayout);
}

void InfoDialog::on_closeButton_clicked() {
    this->close();
}
