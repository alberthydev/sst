#include "session_manager.h"

SessionManager::SessionManager() : m_currentUserId(0) {}

SessionManager& SessionManager::getInstance() {
    static SessionManager instance;
    return instance;
}

void SessionManager::login(int userId, const QString &name, const QString &userType) {
    this->m_currentUserId = userId;
    this->m_currentUserName = name;
    this->m_currentUserType = userType;
}

void SessionManager::logout() {
    this->m_currentUserId = 0;
    this->m_currentUserName.clear();
    this->m_currentUserType.clear();
}

int SessionManager::getCurrentUserId() const {
    return m_currentUserId;
}

QString SessionManager::getCurrentUserName() const {
    return m_currentUserName;
}

QString SessionManager::getCurrentUserType() const {
    return m_currentUserType;
}

bool SessionManager::isUserLoggedIn() const {
    return m_currentUserId > 0;
}
