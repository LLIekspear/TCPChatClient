#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

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

class UserInfoDialog : public QDialog {
    Q_OBJECT

public:
    UserInfoDialog(QWidget *parent=nullptr, QString name="", QString ip="", QString dateTime="", QString status="");
    ~UserInfoDialog();

public slots:
    void on_returnButton_clicked();

private:
    void initializeWidgets(QString ip, QString dateTime, QString status);
    void initializeLayout();
    QPushButton *returnButton;
    QLabel *ipLabel;
    QLabel *dateTimeLabel;
    QLabel *statusLabel;
    QVBoxLayout *verticalLayout;
};

#endif // USERINFODIALOG_H
