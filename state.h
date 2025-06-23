#ifndef STATE_H
#define STATE_H

#include "call_state.h"
#include "call.h"

class OpenState : public CallState {
public:
    QString getName() const override { return "Aberto"; }
    void assignTechnician(Call* call, int techId) override;
};

class OngoingState : public CallState {
public:
    QString getName() const override { return "Em Andamento"; }
    void assignTechnician(Call* Call, int technicianId) override;
    void close(Call* call) override;
};

class CloseState : public CallState {
public:
    QString getName() const override { return "Fechado"; }
    void assignTechnician(Call* call, int techId);
};

#endif // STATE_H
