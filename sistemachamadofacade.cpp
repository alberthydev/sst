#include "sistemachamadofacade.h"
#include "databasemanager.h"
#include "chamadoinfo.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

SistemaChamadosFacade::SistemaChamadosFacade(QObject *parent) : QObject(parent) {}

void SistemaChamadosFacade::abrirNovoChamado(const QString &descricao, int idSolicitante) {
    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("INSERT INTO chamados (descricao, status, id_solicitante) VALUES (?, ?, ?)");
    query.addBindValue(descricao);
    query.addBindValue("Aberto"); // Status inicial
    query.addBindValue(idSolicitante);

    if (query.exec()) {
        int idNovoChamado = query.lastInsertId().toInt();
        QString confirmacao = QString("Chamado #%1 aberto com sucesso!").arg(idNovoChamado);
        emit chamadoCriadoComSucesso(confirmacao); // Emite um sinal de sucesso!
    } else {
        QString erro = "Falha ao salvar no banco: " + query.lastError().text();
        emit falhaNaOperacao(erro); // Emite um sinal de falha!
    }
}

void SistemaChamadosFacade::atualizarStatusDoChamado(int idChamado, const QString &novoStatus) {
    // A lógica de negócio (se a mudança de estado é permitida)
    // estaria aqui ou, melhor ainda, no padrão State!
    // Por enquanto, vamos fazer a atualização direta.
    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("UPDATE chamados SET status = ? WHERE id = ?");
    query.addBindValue(novoStatus);
    query.addBindValue(idChamado);

    if(!query.exec() || query.numRowsAffected() == 0){
        emit falhaNaOperacao("Falha ao atualizar o chamado #" + QString::number(idChamado));
    } else {
        emit chamadoCriadoComSucesso("Chamado #" + QString::number(idChamado) + " atualizado para " + novoStatus);
    }
}

void SistemaChamadosFacade::solicitarListaDeChamados(){
    QList<ChamadoInfo> lista;
    QSqlQuery query(DatabaseManager::getInstance().getConnection());



}
