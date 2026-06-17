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

        Trade t;
        t.symbol = event.symbol;
        t.entry_date = data_->get_latest_bar(event.symbol).date;
        t.entry_price = event.fill_price;
        t.quantity = event.quantity;
        t.total_commission = event.commission;
        open_trades_[event.symbol] = t;
    } else {
        positions_[event.symbol] -= event.quantity;
        current_cash_ += event.quantity * event.fill_price - event.commission;

        if (open_trades_.count(event.symbol)) {
            Trade& t = open_trades_[event.symbol];
            t.exit_date = data_->get_latest_bar(event.symbol).date;
            t.exit_price = event.fill_price;
            t.total_commission += event.commission;
            t.pnl = (t.exit_price - t.entry_price) * t.quantity - t.total_commission;
            trade_log_.push_back(t);
            open_trades_.erase(event.symbol);
        }
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




void FixedFractionalPortfolio::on_signal(const SignalEvent& event) {
    auto order = std::make_unique<OrderEvent>();
    order->symbol = event.symbol;
    order->order_type = OrderType::MARKET;
    order->limit_price = 0.0;

    if (event.direction == "BUY") {
        // Compute current equity = cash + market value of all holdings
        double equity = current_cash_;
        for (const auto& [symbol, shares] : positions_) {
            if (shares == 0) continue;
            equity += shares * data_->get_latest_bar(symbol).close;
        }

        // Size the position
        double target_dollars = equity * risk_per_trade_;
        double price = data_->get_latest_bar(event.symbol).close;
        int quantity = static_cast<int>(target_dollars / price);

        if (quantity == 0) return;

        order->direction = "BUY";
        order->quantity = quantity;

    } else if (event.direction == "EXIT") {
        if (positions_[event.symbol] == 0) return;
        order->direction = "SELL";
        order->quantity = positions_[event.symbol];
    }

    events_->push(std::move(order));
}

void FixedFractionalPortfolio::on_fill(const FillEvent& event) {
    if (event.direction == "BUY") {
        positions_[event.symbol] += event.quantity;
        current_cash_ -= event.quantity * event.fill_price + event.commission;

        Trade t;
        t.symbol = event.symbol;
        t.entry_date = data_->get_latest_bar(event.symbol).date;
        t.entry_price = event.fill_price;
        t.quantity = event.quantity;
        t.total_commission = event.commission;
        open_trades_[event.symbol] = t;
    } else {
        positions_[event.symbol] -= event.quantity;
        current_cash_ += event.quantity * event.fill_price - event.commission;

        if (open_trades_.count(event.symbol)) {
            Trade& t = open_trades_[event.symbol];
            t.exit_date = data_->get_latest_bar(event.symbol).date;
            t.exit_price = event.fill_price;
            t.total_commission += event.commission;
            t.pnl = (t.exit_price - t.entry_price) * t.quantity - t.total_commission;
            trade_log_.push_back(t);
            open_trades_.erase(event.symbol);
        }
    }
}

void FixedFractionalPortfolio::update_timeindex() {
    double equity = current_cash_;
    for (const auto &[symbol,shares]: positions_) {
        if (shares == 0)continue;
        double price = data_->get_latest_bar(symbol).close;
        equity += shares * price;
    }
    equity_curve_.push_back(equity);
}

