#ifndef USER_INFO_H
#define USER_INFO_H

#include <QString>

struct UserInfo {
    int id;
    QString name;
    QString email;
    QString type;
    bool isActive;
};

#endif // USER_INFO_H
