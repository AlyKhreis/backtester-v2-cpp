#include "metrics.h"
double calculate_sharpe(const std::vector<double>& equity_curve) {
    if (equity_curve.size() < 2) return 0;

    std::vector<double> returns;
    for (int i = 1; i < equity_curve.size(); i++) {
        double daily_return = (equity_curve[i] - equity_curve[i-1]) / equity_curve[i-1];
        returns.push_back(daily_return);
    }

    double sum = 0;
    for (double r : returns) sum += r;
    double mean = sum / returns.size();


    double variance = 0;
    for (double r : returns) variance += (r - mean) * (r - mean);
    variance /= returns.size();
    double std_dev = std::sqrt(variance);

    if (std_dev == 0) return 0;


    return (mean / std_dev) * std::sqrt(252);
}

double calculate_max_drawdown(const std::vector<double>& equity_curve) {
    if (equity_curve.empty()) return 0;

    double peak = equity_curve[0];
    double max_dd = 0;

    for (double value : equity_curve) {
        if (value > peak) peak = value;
        double drawdown = (value - peak) / peak;
        if (drawdown < max_dd) max_dd = drawdown;
    }

    return max_dd * 100;
}

double calculate_cagr(const std::vector<double> &equity_curve) {
    if (equity_curve.size() < 2) return 0;
    if (equity_curve.front() == 0) return 0;

    double years = equity_curve.size() / 252.0;
    return std::pow(equity_curve.back() / equity_curve.front(), 1.0 / years) - 1.0;
}


double calculate_winrate(const std::vector<Trade> &trades) {
    if (trades.empty()) return 0;
    int wins {0};
    for (const auto& t : trades) {
        if (t.pnl > 0) wins++;
    }
    return static_cast<double>(wins) / trades.size();
}

double calculate_profit_fact(const std::vector<Trade> &trades) {
    double gross_profit {0.0};
    double gross_loss {0.0};
    for (const auto& t : trades) {
        if (t.pnl>0) gross_profit+=t.pnl;
        else gross_loss -= t.pnl;
    }
    if (gross_loss ==0) return 0.0;
    return gross_profit/gross_loss;
}
