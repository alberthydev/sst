#ifndef CALL_H
#define CALL_H

#include <memory>
#include <QString>
#include "call_state.h"
#include <QDateTime>

class Call {
public:
    Call();
    void startState();

    void assignTechnician(int techId);
    void transitionTo(std::unique_ptr<CallState> newState);
    void close();
    void setClosingDate(const QDateTime &date) { m_closing_date = date; }
    QDateTime closingDate() const { return m_closing_date; }

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }
    void setTechId(int id) { m_tech_id = id; }
    int techId() const { return m_tech_id; }

private:
    std::unique_ptr<CallState> m_currentState;
    int m_id = 0;
    QString m_status;
    int m_tech_id = 0;
    QDateTime m_closing_date;

};


#endif // CALL_H
