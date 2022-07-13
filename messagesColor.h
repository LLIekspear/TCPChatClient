#ifndef MESSAGESCOLOR_H
#define MESSAGESCOLOR_H

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
#include <QColorDialog>

class MessagesDialog : public QDialog {
    Q_OBJECT

public:
    MessagesDialog(QWidget *parent=nullptr);
    ~MessagesDialog();
    QPushButton *messageColorButton;
    QPushButton *infoUserColorButton;
    QPushButton *acceptButton;
    QColor messageColor=Qt::black;
    QColor infoUsersColor=Qt::black;

public slots:
    void on_messageColorButton_clicked();
    void on_infoUserColorButton_clicked();
    void on_acceptButton_clicked();

private:
    void initializeWidgets();
    void initializeLayout();

    QHBoxLayout *messageColorHorizontalLayout;
    QHBoxLayout *infoUserHorizontalLayout;
    QVBoxLayout *verticalLayout;
};
#endif // MESSAGESCOLOR_H
