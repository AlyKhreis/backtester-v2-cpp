#ifndef BACKTESTER_ENGINE_H
#define BACKTESTER_ENGINE_H
#include <memory>
#include <queue>
#include "portfolio.h"
#include "data_handler.h"
#include "event.h"
#include "execution_handler.h"
#include "strategy.h"


class BacktestEngine {
public:
    BacktestEngine(DataHandler *data, Strategy *strategy ,ExecutionHandler *execution, Portfolio *portfolio
        ,std::queue<std::unique_ptr<Event>> *events):
    data_(data), strategy_(strategy), execution_(execution), portfolio_(portfolio), events_(events) {}
    int get_event_count() const { return event_count_; }

    void run();
private:
    void process_event(Event &event);
    DataHandler* data_;
    Strategy* strategy_;
    Portfolio* portfolio_;
    ExecutionHandler* execution_;
    std::queue<std::unique_ptr<Event>>* events_;
    int event_count_{0};
};



#endif
