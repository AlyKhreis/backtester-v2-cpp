#ifndef BACKTESTER_EXECUTION_HANDLER_H
#define BACKTESTER_EXECUTION_HANDLER_H
#include "data_handler.h"
#include <algorithm>


class ExecutionHandler {
public:
    ExecutionHandler(DataHandler *data, std::queue<std::unique_ptr<Event>>* events)
        : data_(data), events_(events) {};
    virtual void execute_order(const OrderEvent& event) = 0;
    virtual ~ExecutionHandler() = default;
protected:
    DataHandler *data_;
    std::queue<std::unique_ptr<Event>> *events_;
};

class SimulatedExecutionHandler: public ExecutionHandler {
public:
    SimulatedExecutionHandler(DataHandler *data, std::queue<std::unique_ptr<Event>> *events, double slippage_pct =0.0005,
        double commission_per_share = 0.005, double min_commission = 1.0)
    : ExecutionHandler(data, events),slippage_pct_(slippage_pct), commission_per_share_(commission_per_share),
       min_commission_(min_commission) {};
    void execute_order(const OrderEvent& event) override;
private:
    double slippage_pct_;
    double commission_per_share_;
    double min_commission_;
};


#endif
