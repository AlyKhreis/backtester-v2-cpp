#ifndef BACKTESTER_EXECUTION_HANDLER_H
#define BACKTESTER_EXECUTION_HANDLER_H
#include "data_handler.h"


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
    SimulatedExecutionHandler(DataHandler *data, std::queue<std::unique_ptr<Event>> *events)
    : ExecutionHandler(data, events) {};
    void execute_order(const OrderEvent& event) override;

};


#endif
