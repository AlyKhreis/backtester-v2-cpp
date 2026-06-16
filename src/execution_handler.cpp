#include "execution_handler.h"



void SimulatedExecutionHandler::execute_order(const OrderEvent& event) {
    auto fill = std::make_unique<FillEvent>();
    fill->symbol = event.symbol;
    fill->fill_price = data_->get_latest_bar(event.symbol).close;
    fill->direction= event.direction;
    fill->quantity = event.quantity;
    fill->commission = 0.0;
    fill->slippage = 0.0;
    events_ -> push(std::move(fill));
}