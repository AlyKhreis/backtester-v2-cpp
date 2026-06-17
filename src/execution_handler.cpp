#include "execution_handler.h"



void SimulatedExecutionHandler::execute_order(const OrderEvent& event) {
    double close = data_->get_latest_bar(event.symbol).close;
    auto fill = std::make_unique<FillEvent>();
    fill->symbol = event.symbol;
    if (event.direction == "BUY") {
        fill->fill_price = close *(1.0 + slippage_pct_) ;
    }else {
        fill->fill_price = close *(1.0 - slippage_pct_) ;
    }
    fill->direction= event.direction;
    fill->quantity = event.quantity;
    fill->commission = std::max(min_commission_, commission_per_share_ * fill->quantity);
    fill->slippage = close * slippage_pct_;;
    events_ -> push(std::move(fill));
}