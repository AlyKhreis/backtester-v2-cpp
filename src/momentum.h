#ifndef BACKTESTER_MOMENTUM_H
#define BACKTESTER_MOMENTUM_H

#include <string>
#include "strategy.h"

class Momentum : public Strategy {
public:
    Momentum(DataHandler* data,
             std::queue<std::unique_ptr<Event>>* events,
             int period,
             const std::string& symbol)
        : Strategy(data, events),
          period_(period),
          symbol_(symbol),
          position_("NONE") {}

    void calculate_signals(const MarketEvent& event) override;

private:
    int period_;
    std::string symbol_;
    std::string position_;
};





#endif
