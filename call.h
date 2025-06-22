#ifndef CALL_H
#define CALL_H

#include <memory>
#include <QString>
#include "call_state.h"

class Call {
public:
    // Construtor que define o estado inicial com base no status do banco
    Call();
    void startState();

    // Delega as ações para o objeto de estado atual
    void assignTechnician(int techId) {
        if(m_currentState) m_currentState->assignTechnician(this, techId);
    }

    // Método para os estados poderem mudar o estado do Chamado
    void transitionTo(std::unique_ptr<CallState> newState);

    // Getters e Setters para as propriedades do chamado
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }

    void setTechId(int id) { m_tech_id = id; }
    int techId() const { return m_tech_id; }

    // ... outros getters e setters para titulo, descricao, etc.

private:
    std::unique_ptr<CallState> m_currentState;

    // Propriedades do chamado
    int m_id = 0;
    QString m_status;
    int m_tech_id = 0;
    // ... etc ...
};


#endif // CALL_H
