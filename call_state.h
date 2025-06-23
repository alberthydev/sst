#ifndef CALL_STATE_H
#define CALL_STATE_H

#include <QDebug>

class Call;

class CallState {
public:
    virtual ~CallState() {}
    virtual QString getName() const = 0;

    virtual void assignTechnician(Call* call, int techId) {
        qWarning() << "Não é possível atribuir um técnico no estado atual:" << getName();
    }

    virtual void close(Call* call) {
        qWarning() << "Não é possível fechar o chamado no estado atual:" << getName();
    }
};

#endif // CALL_STATE_H
