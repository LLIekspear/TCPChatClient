#include "fileSendDialog.h"
#include <QHBoxLayout>

FileSendDialog::FileSendDialog(QWidget *parent, int number) : QDialog(parent) {
    initializeWidgets();
    setModal(false);

    initializeLayout();
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    numbeR=number;
    setWindowTitle(tr("Отправка файла"));
    connect(sendButton, SIGNAL(clicked()), parent, SLOT(on_sendFile()));
    connect(selectFileButton, SIGNAL(clicked()), this, SLOT(on_selectFile()));

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(on_acceptButton_clicked()));
    connect(noButton, SIGNAL(clicked()), this, SLOT(on_noButton_clicked()));
    acceptButton->hide();
    noButton->hide();
}

void FileSendDialog::on_acceptButton_clicked() {
    clicked=1;
    progressBar->show();
    QString dir=QFileDialog::getExistingDirectory(this, tr("Выбор директории"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isNull()) {
        progressBar->setValue(10);
        if(QFileInfo(dir+"/"+filename_new+".txt").exists()) {
            int i=1;
            progressBar->setValue(15);
            bool numberFound=false;
            while(numberFound==false) {
                QString filename_copy=filename_new;
                filename_copy.append(QString::number(i)+".txt");
                if(!QFileInfo(dir+"/"+filename_copy).exists()) {
                    numberFound=true;
                    filename_new=filename_copy;
                    break;
                }
                ++i;
            }
            progressBar->setValue(20);

            QFile file(dir+"/"+filename_new);
            if(file.open(QIODevice::ReadWrite)) {
                progressBar->setValue(25);
                QTextStream stream(&file);
                stream<<packet;
                file.close();
            }
            progressBar->setValue(100);
            fileSavingReady=true;
        } else {
            progressBar->setValue(20);
            filename_new.append(".txt");
            QFile file(dir+"/"+filename_new);
            if(file.open(QIODevice::ReadWrite)) {
                progressBar->setValue(30);
                QTextStream stream(&file);
                stream<<packet;
                file.close();

                progressBar->setValue(100);
                fileSavingReady=true;
            }
        }
    }
}

void FileSendDialog::on_noButton_clicked() {
    clicked=2;
    this->close();
}

FileSendDialog::~FileSendDialog() {
    delete fileLabel;
    delete sendButton;
    delete selectFileButton;
    delete verticalLayout;
}

void FileSendDialog::initializeWidgets() {
    fileLabel=new QLabel(tr("Выбранный файл: пусто"));
    acceptButton= new QPushButton(tr("Принять"));
    noButton=new QPushButton(tr("Отклонить"));
    selectFileButton=new QPushButton(tr("Выбрать файл"));
    sendButton=new QPushButton(tr("Отправить файл"));
    progressBar=new QProgressBar(this);
}

void FileSendDialog::initializeLayout() {
    verticalLayout=new QVBoxLayout();
    verticalLayout->addWidget(fileLabel);
    verticalLayout->addWidget(selectFileButton);
    verticalLayout->addWidget(acceptButton);
    verticalLayout->addWidget(noButton);
    verticalLayout->addWidget(sendButton);
    verticalLayout->addWidget(progressBar);
    setLayout(verticalLayout);
}

void FileSendDialog::on_selectFile() {
    QString path=QFileDialog::getOpenFileName(this, tr("Open File"), "/home/user/Desktop", tr("Text Files (*.txt)"));
    QFile file(path);
    QFileInfo fileInfo(file.fileName());
    filename=fileInfo.baseName();
    bytes=fileInfo.size();
    fileLabel->setText("Выбранный файл: "+filename);
    fileLabel->setText(filename);
    progressBar->setValue(10);

    QByteArray block;
    file.open(QIODevice::ReadOnly);
    QDataStream out(&block, QIODevice::WriteOnly);
    progressBar->setValue(20);
    out.setVersion(QDataStream::Qt_4_1);
    out<<file.readAll();
    //Переход на позицию перед нулем
//    out.device()->seek(0);
    progressBar->setValue(25);
    file_bity=block;
    progressBar->setValue(30);
    file.close();

}

void FileSendDialog::on_sendButton_clicked() {
    //
}
