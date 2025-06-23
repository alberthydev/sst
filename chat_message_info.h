#ifndef CHAT_MESSAGE_INFO_H
#define CHAT_MESSAGE_INFO_H

#include <QString>
#include <QDateTime>

struct ChatMessageInfo {
    QString senderName;
    QString message;
    QDateTime timestamp;
};

#endif // CHAT_MESSAGE_INFO_H
