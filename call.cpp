#include "call.h"
#include "state.h"

Call::Call() {}

void Call::startState() {
    if (m_status == "Aberto") {
        m_currentState = std::make_unique<OpenState>();
    } else if (m_status == "Em Andamento") {
        m_currentState = std::make_unique<OngoingState>();
    } else if (m_status == "Fechado") {
        m_currentState = std::make_unique<CloseState>();
    } else {
        m_currentState = std::make_unique<OpenState>();
    }
}

void Call::assignTechnician(int techId) {
    if(m_currentState) {
        m_currentState->assignTechnician(this, techId);
    }
}

void Call::close()
{
    if (m_currentState) {
        m_currentState->close(this);
    }
}

void Call::transitionTo(std::unique_ptr<CallState> newState) {
    qDebug() << "Transicionando do estado" << m_currentState->getName()
    << "para" << newState->getName();
    m_currentState = std::move(newState);
}
