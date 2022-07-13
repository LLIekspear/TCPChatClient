#ifndef FILESENDDIALOG_H
#define FILESENDDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QtNetwork/QTcpServer>
#include <QMainWindow>
#include <QProgressBar>
#include <QFile>
#include <QFileDialog>

class FileSendDialog : public QDialog {
    Q_OBJECT

public:
    FileSendDialog(QWidget *parent=nullptr, int number=0);
    ~FileSendDialog();
    QByteArray file_bity;
    QString filename;
    QPushButton *selectFileButton;
    QPushButton *sendButton;
    QPushButton *acceptButton;
    QPushButton *noButton;
    QProgressBar *progressBar;
    QLabel *fileLabel;
    int numbeR;
    int bytes;
    QByteArray packet;
    QString filename_new;
    bool fileSavingReady=false;
    int clicked=0;

public slots:
    void on_sendButton_clicked();
    void on_selectFile();
    void on_acceptButton_clicked();
    void on_noButton_clicked();

private:
    void initializeWidgets();
    void initializeLayout();

    QVBoxLayout *verticalLayout;
};

#endif // FILESENDDIALOG_H
