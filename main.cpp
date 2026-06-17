#include <iostream>
#include <queue>
#include <memory>
#include "event.h"
#include "data_handler.h"
#include "ma_crossover.h"
#include "portfolio.h"
#include "execution_handler.h"
#include "engine.h"
#include "rsi.h"
#include "momentum.h"
#include "metrics.h"
#include <fstream>



void export_trades_to_csv(const std::vector<Trade>& trades, const std::string& path) {
    std::ofstream out(path);
    out << "symbol,entry_date,exit_date,entry_price,exit_price,quantity,pnl,commission\n";
    for (const auto& t : trades) {
        out << t.symbol << "," << t.entry_date << "," << t.exit_date << ","
            << t.entry_price << "," << t.exit_price << "," << t.quantity << ","
            << t.pnl << "," << t.total_commission << "\n";
    }
}

void export_equity_to_csv(const std::vector<double>& curve, const std::string& path) {
    std::ofstream out(path);
    out << "bar,equity\n";
    for (size_t i = 0; i < curve.size(); i++) {
        out << i << "," << curve[i] << "\n";
    }
}

int main() {
    std::queue<std::unique_ptr<Event>> events;

    HistoricalCSVDataHandler data(&events,
        "/Users/aly/Desktop/CPPProjects/backtester-v2-cpp/data/AAPL.csv",
        "AAPL");

    std::cout << "Select Strategy:\n";
    std::cout << "1. Moving Average Crossover\n";
    std::cout << "2. RSI\n";
    std::cout << "3. Momentum\n";
    std::cout << "Choice: ";
    int choice;
    std::cin >> choice;

    Strategy* strategy = nullptr;

    if (choice == 1) {
        int fast, slow;
        std::cout << "Fast period: ";  std::cin >> fast;
        std::cout << "Slow period: ";  std::cin >> slow;
        strategy = new MovingAverageCrossover(&data, &events, fast, slow, "AAPL");
    } else if (choice == 2) {
        int period;
        double ob, os;
        std::cout << "Period: ";      std::cin >> period;
        std::cout << "Overbought: ";  std::cin >> ob;
        std::cout << "Oversold: ";    std::cin >> os;
        strategy = new RSI(&data, &events, period, ob, os, "AAPL");
    } else {
        int period;
        std::cout << "Period: ";  std::cin >> period;
        strategy = new Momentum(&data, &events, period, "AAPL");
    }

    FixedFractionalPortfolio portfolio(&data, &events, 100000.0, 0.95);
    SimulatedExecutionHandler execution(&data, &events);

    BacktestEngine engine(&data, strategy, &execution, &portfolio, &events);

    engine.run();

    const auto& curve = portfolio.get_equity_curve();
    std::cout << "\nFinal equity: $" << curve.back() << "\n";
    std::cout << "Total return: " << (curve.back() / 100000.0 - 1.0) * 100 << "%\n";
    std::cout << "Bars: " << curve.size() << "\n";

    std::cout << "Sharpe ratio: " << calculate_sharpe(curve) << "\n";
    std::cout << "Max drawdown: " << calculate_max_drawdown(curve) << "%\n";
    std::cout << "CAGR: " << calculate_cagr(curve) * 100 << "%\n";


    export_trades_to_csv(portfolio.get_trade_log(), "trades.csv");
    export_equity_to_csv(curve, "equity.csv");
    std::cout << "\nExported trades.csv and equity.csv\n";
    std::cout << "Trades: " << portfolio.get_trade_log().size() << "\n";

    std::cout << "Win rate: " << calculate_winrate(portfolio.get_trade_log()) * 100 << "%\n";
    std::cout << "Profit factor: " << calculate_profit_fact(portfolio.get_trade_log()) << "\n";

    delete strategy;
    return 0;
}