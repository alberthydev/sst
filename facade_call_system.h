#ifndef FACADE_CALL_SYSTEM_H
#define FACADE_CALL_SYSTEM_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "call_info.h"
#include "call.h"
#include "state.h"

class facade_call_system : public QObject
{
    Q_OBJECT

public:
    explicit facade_call_system(QObject *parent = nullptr);

    Q_INVOKABLE int callCount(const QString &status = QString());
    void getTechList();
    void assignTechnicianToCaller(int callId, int techId);

public slots:
    void getCallList(const QString &status = QString());
    void getCallDetails(int callId);
    void requestTechnicianList();
    void assignTechnicianToCall(int ticketId, int technicianId);

signals:
    void readyCallList(const QList<CallInfo> &calls);
    void operationFailed(const QString &error);
    void readyCallDetails(const CallInfo &info);
    void technicianListReady(const QMap<int, QString> &technicians);
    void callUpdatedSuccessfully(const QString &message);

private:
    CallInfo info;
};

#endif // FACADE_CALL_SYSTEM_H
