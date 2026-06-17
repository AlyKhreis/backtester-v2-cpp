#include "engine.h"


void BacktestEngine::process_event(Event &event) {
    switch (event.type()) {
        case EventType:: MARKET:
            strategy_->calculate_signals(static_cast<MarketEvent&>(event));
            portfolio_->update_timeindex();
            break;
        case EventType::SIGNAL:
            portfolio_->on_signal(static_cast<SignalEvent&>(event));
            break;
        case EventType::ORDER:
            execution_->execute_order(static_cast<OrderEvent&>(event));
            break;
        case EventType::FILL:
            portfolio_->on_fill(static_cast<FillEvent&>(event));
            break;
    }
}

void BacktestEngine::run() {
    while (data_->has_more_data() || !events_->empty()) {
        if (events_->empty()) {
            data_->update_bars();
        }
        while (!events_->empty()) {
            auto event = std::move(events_->front());
            events_-> pop();
            process_event(*event);
        }
    }
}