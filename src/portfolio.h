#ifndef BACKTESTER_PORTFOLIO_H
#define BACKTESTER_PORTFOLIO_H
#include <queue>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "event.h"
#include "data_handler.h"
#include <iostream>

class Portfolio {
public:
    Portfolio(DataHandler* data,
              std::queue<std::unique_ptr<Event>>* events,
              double initial_cash)
        : data_(data),
          events_(events),
          initial_cash_(initial_cash),
          current_cash_(initial_cash) {}
    const std::vector<double>& get_equity_curve() const { return equity_curve_; }
    virtual void on_signal(const SignalEvent& event) = 0;
    virtual void on_fill(const FillEvent& event) = 0;
    virtual void update_timeindex() = 0;

    virtual ~Portfolio() = default;

protected:
    DataHandler* data_;
    std::queue<std::unique_ptr<Event>>* events_;
    double initial_cash_;
    double current_cash_;
    std::unordered_map<std::string, int> positions_;
    std::vector<double> equity_curve_;
};


class NaivePortfolio : public Portfolio {
public:
    NaivePortfolio(DataHandler* data,
                   std::queue<std::unique_ptr<Event>>* events,
                   double initial_cash)
        : Portfolio(data, events, initial_cash) {}

    void on_signal(const SignalEvent& event) override;
    void on_fill(const FillEvent& event) override;
    void update_timeindex() override;
};
#endif
