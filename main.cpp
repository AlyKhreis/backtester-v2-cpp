#include <iostream>
#include <queue>
#include <memory>
#include "event.h"
#include "data_handler.h"

int main() {
    std::queue<std::unique_ptr<Event>> events;

    HistoricalCSVDataHandler data(&events,
                                  "/Users/aly/Desktop/CPPProjects/backtester-v2-cpp/data/AAPL.csv",
                                  "AAPL");

    int bar_count = 0;
    while (data.has_more_data()) {
        data.update_bars();

        // pop the MarketEvent the handler just pushed
        auto event = std::move(events.front());
        events.pop();

        Bar latest = data.get_latest_bar("AAPL");
        std::cout << latest.date << "  close: $" << latest.close << "\n";
        bar_count++;
    }

    std::cout << "\nTotal bars: " << bar_count << "\n";
    return 0;
}