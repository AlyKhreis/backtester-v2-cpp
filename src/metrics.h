#ifndef BACKTESTER_METRICS_H
#define BACKTESTER_METRICS_H
#include <cmath>
#include <vector>
#include <algorithm>

double calculate_sharpe(const std::vector<double>& equity_curve);
double calculate_max_drawdown(const std::vector<double>& equity_curve);
double calculate_cagr(const std::vector<double>& equity_curve);



#endif //BACKTESTER_METRICS_H
