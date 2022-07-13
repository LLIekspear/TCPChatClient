#ifndef USERNAMEDIALOG_H
#define USERNAMEDIALOG_H

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

class UsernameDialog : public QDialog {
    Q_OBJECT

public:
    UsernameDialog(QWidget *parent=nullptr);
    ~UsernameDialog();
    QPushButton *acceptButton;
    QLineEdit *usernameLineEdit;

public slots:
    void on_acceptButton_clicked();

private:
    void initializeWidgets();
    void initializeLayout();

    QLabel *usernameLabel;
    QHBoxLayout *usernameHorizontalLayout;
    QVBoxLayout *verticalLayout;
};

#endif // USERNAMEDIALOG_H
