#ifndef REQUESTER_H
#define REQUESTER_H

#include <QWidget>
#include <QList>
#include <QTableWidgetItem>
#include "facade_call_system.h"
#include "principal.h"

namespace Ui {
class requester;
}

class requester : public QWidget
{
    Q_OBJECT

public:
    explicit requester(QWidget *parent = nullptr);
    ~requester();

private slots:
    void on_openCallBtn_clicked();
    void on_goBackNewCallBtn_clicked();
    void on_goBackRequesterCallsBtn_clicked();
    void on_requesterCallsBtn_clicked();
    void on_goBackGenericCallBtn_clicked();
    void on_sendCallBtn_clicked();
    void onOperationFailed(const QString &errorMessage);
    void onCallCreationSuccess(const QString &message);
    void populateCallsTable(const QList<CallInfo> &calls);
    void onMyCallDoubleClicked(QTableWidgetItem *item);
    void displayCallDetails(const CallInfo &info);
    void displayChatMessages(const QList<ChatMessageInfo> &messages);

    void on_sendMessageBtn_clicked();

    void on_logoutRequesterBtn_clicked();

private:
    Ui::requester *ui;
    Principal *loginWindow;
    facade_call_system *m_facade;
    int m_currentCallId;
};

#endif // REQUESTER_H
