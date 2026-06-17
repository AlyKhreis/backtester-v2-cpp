#include <iostream>
#include <queue>
#include <memory>
#include "event.h"
#include "data_handler.h"
#include "ma_crossover.h"
#include "portfolio.h"
#include "execution_handler.h"
#include "engine.h"

int main() {
    std::queue<std::unique_ptr<Event>> events;

    HistoricalCSVDataHandler data(&events,
        "/Users/aly/Desktop/CPPProjects/backtester-v2-cpp/data/AAPL.csv",
        "AAPL");

    MovingAverageCrossover strategy(&data, &events, 10, 50, "AAPL");
    NaivePortfolio portfolio(&data, &events, 100000.0);
    SimulatedExecutionHandler execution(&data, &events);

    BacktestEngine engine(&data, &strategy, &execution, &portfolio, &events);

    engine.run();

    const auto& curve = portfolio.get_equity_curve();
    std::cout << "Final equity: $" << curve.back() << "\n";
    std::cout << "Total return: " << (curve.back() / 100000.0 - 1.0) * 100 << "%\n";
    std::cout << "Bars: " << curve.size() << "\n";

    return 0;
}