#ifndef USER_H
#define USER_H

#include <QByteArray>
#include <QVector>
#include <QString>
#include <QImage>

class User {
public:
    User();
    QString username="Test";
    QImage icon;
    QString id;
    QString ip;
    QString connected_time;
    QString status;

private:
//    QString username="Test";
    QByteArray user_id;

public:
    void set_user_id(QByteArray);
    void set_username(QString);
    QByteArray get_user_id();
    QString get_username();
};

#endif // USER_H
