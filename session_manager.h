#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <QString>

class SessionManager {
public:
    // O método para acessar a única instância
    static SessionManager& getInstance();

    // Método a ser chamado pela tela de login após a validação
    void login(int userId, const QString& name, const QString& userType);
    // Método para limpar os dados ao deslogar
    void logout();

    // Getters para obter os dados do usuário logado de qualquer lugar
    int getCurrentUserId() const;
    QString getCurrentUserName() const;
    QString getCurrentUserType() const;
    bool isUserLoggedIn() const;

    // Prevenção de cópias (padrão do Singleton)
    SessionManager(const SessionManager&) = delete;
    void operator=(const SessionManager&) = delete;

private:
    // Construtor privado
    SessionManager();

    // Dados do usuário da sessão atual
    int m_currentUserId;
    QString m_currentUserName;
    QString m_currentUserType;
};

#endif // SESSION_MANAGER_H
