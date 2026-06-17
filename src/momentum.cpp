#include "momentum.h"


void Momentum::calculate_signals(const MarketEvent& event) {
    auto bars = data_->get_latest_bars(symbol_, period_ + 1);
    if (bars.size() < period_ + 1) return;

    double past_price = bars.front().close;
    double current_price = bars.back().close;

    if (current_price > past_price && position_ != "LONG") {
        auto signal = std::make_unique<SignalEvent>();
        signal->symbol = symbol_;
        signal->direction = "BUY";
        signal->strength = 1.0;
        events_->push(std::move(signal));
        position_ = "LONG";
    } else if (current_price < past_price && position_ != "NONE") {
        auto signal = std::make_unique<SignalEvent>();
        signal->symbol = symbol_;
        signal->direction = "EXIT";
        signal->strength = 1.0;
        events_->push(std::move(signal));
        position_ = "NONE";
    }
}