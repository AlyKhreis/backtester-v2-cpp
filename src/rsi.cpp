#include "rsi.h"

double RSI::calculate_rsi(const std::vector<Bar>& bars) const {
    double gain_sum = 0.0;
    double loss_sum = 0.0;
    for (size_t i = 1; i < bars.size(); i++) {
        double change = bars[i].close - bars[i-1].close;
        if (change > 0) gain_sum += change;
        else            loss_sum -= change;
    }
    double avg_gain = gain_sum / period_;
    double avg_loss = loss_sum / period_;
    if (avg_loss == 0) return 100.0;
    double rs = avg_gain / avg_loss;
    return 100.0 - (100.0 / (1.0 + rs));
}

void RSI::calculate_signals(const MarketEvent& event) {
    auto bars = data_->get_latest_bars(symbol_, period_ + 1);
    if (bars.size() < period_ + 1) return;

    double rsi = calculate_rsi(bars);

    if (rsi < oversold_ && position_ != "LONG") {
        auto signal = std::make_unique<SignalEvent>();
        signal->symbol = symbol_;
        signal->direction = "BUY";
        signal->strength = 1.0;
        events_->push(std::move(signal));
        position_ = "LONG";
    } else if (rsi > overbought_ && position_ != "NONE") {
        auto signal = std::make_unique<SignalEvent>();
        signal->symbol = symbol_;
        signal->direction = "EXIT";
        signal->strength = 1.0;
        events_->push(std::move(signal));
        position_ = "NONE";
    }
}