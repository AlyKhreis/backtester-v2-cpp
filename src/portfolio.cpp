#include "portfolio.h"



void NaivePortfolio::on_signal(const SignalEvent& event){
    auto order = std::make_unique<OrderEvent>();
    order->symbol= event.symbol;
    order->order_type = OrderType::MARKET;
    order -> limit_price = 0.0;
    if (event.direction=="BUY") {
        order->direction = "BUY";
        order->quantity = 100;
    }else if (event.direction == "EXIT") {
        if (positions_[event.symbol] == 0) return;
        else {
            order->direction = "SELL";
            order->quantity = positions_[event.symbol];
        }
    }
    events_->push(std::move(order));
}


void NaivePortfolio::on_fill(const FillEvent& event) {
    if (event.direction == "BUY") {
        positions_[event.symbol] += event.quantity;
        current_cash_ -= event.quantity * event.fill_price + event.commission;
    }else {
        positions_[event.symbol] -= event.quantity;
        current_cash_ += event.quantity * event.fill_price - event.commission;
    }
}

void NaivePortfolio::update_timeindex() {
    double equity = current_cash_;
    for (const auto &[symbol,shares]: positions_) {
        if (shares == 0)continue;
        double price = data_->get_latest_bar(symbol).close;
        equity += shares * price;
    }
    equity_curve_.push_back(equity);
}
