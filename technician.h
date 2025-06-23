#ifndef TECHNICIAN_H
#define TECHNICIAN_H

#include <QWidget>
#include "facade_call_system.h"
#include "call_info.h"
#include "qtablewidget.h"
#include "chat_message_info.h"
#include "user_info.h"
#include "principal.h"

namespace Ui {
class technician;
}

class technician : public QWidget
{
    Q_OBJECT

public:
    explicit technician(QWidget *parent = nullptr);
    ~technician();
    void updateCountersDashboard();

private slots:
    void on_myCallsBtn_clicked();
    void on_backBtn_clicked();
    void on_backUserBtn_clicked();
    void on_usersBtn_clicked();
    void on_backUserEditBtn_clicked();
    void on_okCallsBtn_clicked();
    void on_newCalls_clicked();
    void on_analysisCalls_clicked();
    void on_completedCalls_clicked();
    void populateTable(const QList<CallInfo> &calls);
    void onItemTableDoubleClick(QTableWidgetItem *item);
    void showCallDetails(const CallInfo &info);
    void on_editResponBtn_clicked();
    void onTechnicianSelected(int technicianId);
    void onCallUpdateSuccess(const QString &message);
    void on_addComentBtn_clicked();
    void displayChatMessages(const QList<ChatMessageInfo> &messages);
    void on_finishCallBtn_clicked();
    void onOperationFailed(const QString &errorMessage);
    void displayUserLists(const QList<UserInfo> &technicians, const QList<UserInfo> &requesters);
    void on_rmvUserBtn_clicked();
    void onOperationSuccess(const QString &message);
    void refreshUserLists();
    void on_editUserBtn_clicked();
    void on_createUserBtn_clicked();
    void populateUserDetailsForm(const UserInfo &info);
    void on_okEditUserBtn_clicked();
    void onTechnicianSelectionChanged();
    void onRequesterSelectionChanged();
    void on_logoutTechnichianBtn_clicked();

private:
    Ui::technician *ui;
    Principal *loginWindow;
    facade_call_system *m_facade;
    int m_currentCallId;
    int m_currentUserEditingId;
};

#endif // TECHNICIAN_H
