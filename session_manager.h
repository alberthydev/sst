#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <QString>

class SessionManager {
public:
    static SessionManager& getInstance();
    void login(int userId, const QString& name, const QString& userType);
    void logout();
    int getCurrentUserId() const;
    QString getCurrentUserName() const;
    QString getCurrentUserType() const;
    bool isUserLoggedIn() const;

    // Garante o padrao singleton
    SessionManager(const SessionManager&) = delete;
    void operator=(const SessionManager&) = delete;

private:
    SessionManager();
    int m_currentUserId;
    QString m_currentUserName;
    QString m_currentUserType;
};

#endif // SESSION_MANAGER_H
