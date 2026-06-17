#ifndef RSI_H
#define RSI_H

#include <string>
#include <vector>
#include "strategy.h"
#include "bar.h"

class RSI : public Strategy {
public:
    RSI(DataHandler* data,
        std::queue<std::unique_ptr<Event>>* events,
        int period,
        double overbought,
        double oversold,
        const std::string& symbol)
        : Strategy(data, events),
          period_(period),
          overbought_(overbought),
          oversold_(oversold),
          symbol_(symbol),
          position_("NONE") {}

    void calculate_signals(const MarketEvent& event) override;

private:
    int period_;
    double overbought_;
    double oversold_;
    std::string symbol_;
    std::string position_;

    double calculate_rsi(const std::vector<Bar>& bars) const;
};

#endif
