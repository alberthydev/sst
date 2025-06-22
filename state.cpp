#include "state.h"
#include "call.h"
#include <QDebug>

// Lógica do estado Aberto
void OpenState::assignTechnician(Call* call, int techId)
{
    if (techId > 0) {
        qDebug() << "Lógica do OpenState: Atribuindo técnico e mudando para Em Andamento.";
        call->setStatus("Em Andamento");
        call->setTechId(techId);
        call->transitionTo(std::make_unique<OngoingState>());
    } else {
        // Se já está aberto e sem técnico, e o usuário seleciona "Nenhum", não faz nada.
        qDebug() << "Lógica do OpenState: Chamado já está sem técnico. Nenhuma ação necessária.";
    }
}

void OngoingState::assignTechnician(Call* call, int techId){
    if (techId > 0) {
        qDebug() << "Lógica do InProgressState: Reatribuindo para o técnico ID" << techId;
        call->setTechId(techId);

    } else {
        qDebug() << "Lógica do InProgressState: Desatribuindo técnico. Voltando para o estado Aberto.";
        call->setStatus("Aberto");
        call->setTechId(0); // 0 representará NULL no banco
        call->transitionTo(std::make_unique<OpenState>());
    }
}

// Lógica do estado Em Andamento
void OngoingState::close(Call* call) {
    qDebug() << "Lógica do EstadoEmAndamento: Fechando o chamado.";
    call->setStatus("Fechado");
    // Aqui você também poderia setar a data_fechamento
    call->transitionTo(std::make_unique<CloseState>());
}

void CloseState::assignTechnician(Call* call, int techId)
{
    // A regra de negócio: não fazer nada!
    qWarning() << "Ação inválida: Não é possível alterar o técnico de um chamado que já está Fechado.";
}
