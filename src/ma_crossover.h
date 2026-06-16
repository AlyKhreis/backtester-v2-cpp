#ifndef BACKTESTER_MA_CROSSOVER_H
#define BACKTESTER_MA_CROSSOVER_H
#include "strategy.h"
#include <string>
#include "bar.h"
#include <vector>
class MovingAverageCrossover : public Strategy {
public:
    MovingAverageCrossover(DataHandler *data, std::queue<std::unique_ptr<Event>>* events,
       int fast_period, int slow_period, const std::string &symbol);
    void calculate_signals(const MarketEvent &event) override;
private:
    double calculate_ma(const std::vector<Bar> &bars, int start, int period) const;
    int fast_period_;
    int slow_period_;
    std::string symbol_;
    std::string position_;
};


#endif