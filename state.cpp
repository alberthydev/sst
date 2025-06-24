#include "state.h"
#include "call.h"
#include <QDebug>

// Lógica do estado Aberto de um chamado
void OpenState::assignTechnician(Call* call, int techId)
{
    if (techId > 0) {
        qDebug() << "Lógica do OpenState: Atribuindo técnico e mudando para Em Andamento.";
        call->setStatus("Em Andamento");
        call->setTechId(techId);
        call->transitionTo(std::make_unique<OngoingState>());
    } else {
        qDebug() << "Lógica do OpenState: Chamado já está sem técnico. Nenhuma ação necessária.";
    }
}

// Lógica do estado Em Andamento
void OngoingState::assignTechnician(Call* call, int techId){
    if (techId > 0) {
        qDebug() << "Lógica do OngoingState: Reatribuindo para o técnico ID" << techId;
        call->setTechId(techId);

    } else {
        qDebug() << "Lógica do OngoingState: Desatribuindo técnico. Voltando para o estado Aberto.";
        call->setStatus("Aberto");
        call->setTechId(0); // 0 = NULL para o nosso banco -> padrão definido para funcionar bem
        call->transitionTo(std::make_unique<OpenState>());
    }
}

void OngoingState::close(Call* call) {
    qDebug() << "Lógica do OngoingState: Fechando o chamado.";
    call->setStatus("Fechado");
    call->setClosingDate(QDateTime::currentDateTimeUtc());
    call->transitionTo(std::make_unique<CloseState>());
}

// Lógica do estado Concluido/Fechado
void CloseState::assignTechnician(Call* call, int techId)
{
    qWarning() << "Ação inválida: Não é possível alterar o técnico de um chamado que já está Fechado.";
}
