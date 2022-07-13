#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

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

class ServerDialog : public QDialog {
    Q_OBJECT

public:
    ServerDialog(QWidget *parent=nullptr);
    ~ServerDialog();
    QPushButton *acceptButton;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;

public slots:
    void on_acceptButton_clicked();

private:
    void initializeWidgets();
    void initializeLayout();

    QLabel *ipLabel;
    QLabel *portLabel;
    QHBoxLayout *ipHorizontalLayout;
    QHBoxLayout *portHorizontalLayout;
    QVBoxLayout *verticalLayout;
};

#endif // SERVERDIALOG_H
