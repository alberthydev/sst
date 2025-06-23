#ifndef FACADE_CALL_SYSTEM_H
#define FACADE_CALL_SYSTEM_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "call_info.h"
#include "chat_message_info.h"
#include "call.h"
#include "state.h"
#include "user_info.h"

class facade_call_system : public QObject
{
    Q_OBJECT

public:
    explicit facade_call_system(QObject *parent = nullptr);

    Q_INVOKABLE int callCount(const QString &status = QString());
    void getTechList();
    void assignTechnicianToCaller(int callId, int techId);

public slots:
    void getCallList(const QString &status = QString(), int technicianId = 0, int requesterId = 0);
    void getCallDetails(int callId);
    void requestTechnicianList();
    void assignTechnicianToCall(int ticketId, int technicianId);
    void requestChatMessages(int callId);
    void sendChatMessage(int callId, const QString &message);
    void closeCall(int callId);
    void requestUserLists();
    void deleteUser(int userId);
    void requestUserDetails(int userId);
    void saveUser(int userId, const QString &name, const QString &email, const QString &password, const QString &type);
    void createNewCall(const QString &title, const QString &type, const QString &priority, const QString &description);
    void validateLogin(const QString &email, const QString &password);

signals:
    void operationFailed(const QString &error);
    void readyCallList(const QList<CallInfo> &calls);
    void readyCallDetails(const CallInfo &info);
    void technicianListReady(const QMap<int, QString> &technicians);
    void callUpdatedSuccessfully(const QString &message);
    void chatMessagesReady(const QList<ChatMessageInfo> &messages);
    void userListsReady(const QList<UserInfo> &technicians, const QList<UserInfo> &requesters);
    void userDeletedSuccessfully(const QString &message);
    void userDetailsReady(const UserInfo &info);
    void userSavedSuccessfully(const QString &message);
    void callCreatedSuccessfully(const QString &message);
    void loginSuccessful(int userId, const QString &name, const QString &userType);


private:
    CallInfo info;
};

#endif // FACADE_CALL_SYSTEM_H
