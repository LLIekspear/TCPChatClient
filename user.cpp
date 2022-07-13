#include "user.h"

User::User() {
    //
}

void User::set_user_id(QByteArray user_id) {
    this->user_id=user_id;
}

void User::set_username(QString name) {
    this->username=name;
}

QByteArray User::get_user_id() {
    return user_id;
}

QString User::get_username() {
    return username;
}
