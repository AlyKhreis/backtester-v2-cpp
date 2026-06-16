#ifndef STRATEGY_H
#define STRATEGY_H

#include <queue>
#include <memory>
#include "event.h"
#include "data_handler.h"

class Strategy {
public:
    Strategy(DataHandler* data, std::queue<std::unique_ptr<Event>>* events)
        : data_(data), events_(events) {}

    virtual void calculate_signals(const MarketEvent& event) = 0;

    virtual ~Strategy() = default;

protected:
    DataHandler* data_;
    std::queue<std::unique_ptr<Event>>* events_;
};

#endif
