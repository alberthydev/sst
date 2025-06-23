#ifndef CALL_INFO_H
#define CALL_INFO_H

#include <QString>
#include <QDateTime>

struct CallInfo {
    int id;
    QString titulo;
    QString descricao;
    QString status;
    QDateTime data_abertura;
    QDateTime data_fechamento;
    QString tipo;
    QString prioridade;
    QString nome_solicitante;
    QString nome_tecnico;
};

#endif // CALL_INFO_H
