#ifndef CUSTOMSTATUSDIALOG_H
#define CUSTOMSTATUSDIALOG_H

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

class CustomStatusDialog : public QDialog {
    Q_OBJECT

public:
    CustomStatusDialog(QWidget *parent=nullptr);
    ~CustomStatusDialog();
    QPushButton *acceptButton;
    QLineEdit *customStatusLineEdit;

public slots:
    void on_acceptButton_clicked();

private:
    void initializeWidgets();
    void initializeLayout();

    QLabel *customStatusLabel;
    QHBoxLayout *statusHorizontalLayout;
    QVBoxLayout *verticalLayout;
};


#endif // CUSTOMSTATUSDIALOG_H
