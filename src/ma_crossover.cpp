#include "ma_crossover.h"


MovingAverageCrossover::MovingAverageCrossover(DataHandler *data,std::queue<std::unique_ptr<Event>> *events,
    int fast_period_, int slow_period_,
    const std::string &symbol)
        :Strategy(data, events),
    fast_period_(fast_period_),
    slow_period_(slow_period_),
    position_("NONE"),
    symbol_(symbol){}




double MovingAverageCrossover::calculate_ma(const std::vector<Bar> &bars, int start, int period) const {
    double sum{};
    for (int i = start; i < start+period; ++i) {
        sum += bars[i].close;
    }
    return sum/ period;
}

void MovingAverageCrossover::calculate_signals(const MarketEvent& event) {
    auto bars = data_->get_latest_bars(symbol_, slow_period_ + 1);
    if (bars.size() < slow_period_ + 1) return;

    double today_fast     = calculate_ma(bars, bars.size() - fast_period_,     fast_period_);
    double yesterday_fast = calculate_ma(bars, bars.size() - fast_period_ - 1, fast_period_);
    double today_slow     = calculate_ma(bars, bars.size() - slow_period_,     slow_period_);
    double yesterday_slow = calculate_ma(bars, bars.size() - slow_period_ - 1, slow_period_);

    bool bullish = (yesterday_fast <= yesterday_slow) && (today_fast >  today_slow);
    bool bearish = (yesterday_fast >= yesterday_slow) && (today_fast <  today_slow);

    if (bullish && position_ != "LONG") {
        auto signal = std::make_unique<SignalEvent>();
        signal->symbol = symbol_;
        signal->direction = "BUY";
        signal->strength = 1.0;
        events_->push(std::move(signal));
        position_ = "LONG";
    } else if (bearish && position_ != "NONE") {
        auto signal = std::make_unique<SignalEvent>();
        signal->symbol = symbol_;
        signal->direction = "EXIT";
        signal->strength = 1.0;
        events_->push(std::move(signal));
        position_ = "NONE";
    }
}
